#pragma once
#include <SFML\Graphics.hpp>
#include "Vector3.h"

/*
* Base class for objects in the scene. Each object has a position, color, and
* custom distance function that returns the distance from any point in space
* to the surface of the object. Each object also has a "negative" boolean
* variable, which when set to true makes the distance function return negative
* values, used for boolean rendering.
*/
class SceneObject
{
public:
	SceneObject(const Vector3& pos_, const sf::Color& c, const bool neg = false) : pos(pos_), color(c), negative(neg) {}
	virtual float distanceFromPoint(const Vector3& p) const { return 0; } // returns the distance from any point in space to the surface of the object.
	sf::Color getColor() const { return color; }
	Vector3 getPosition() const { return pos; }
	void setPosition(const Vector3& newPos) { pos = newPos; }
protected:
	sf::Color color;
	Vector3 pos;
	bool negative;
};

// Sphere object, takes in the additional parameter of radius.
class Sphere : public SceneObject
{
public:
	Sphere(const Vector3& pos_, const float r, const sf::Color& c, const bool neg = false) : SceneObject(pos_, c, neg), radius(r) {}
	float distanceFromPoint(const Vector3& p) const
	{
		float result = Vector3::dist(pos, p) - radius;
		return negative ? -result : result;
	}
	
protected:
	float radius;
};

/*
* Axis-aligned box object, takes in the additional parameter "dimensions:" a vector defining
* the half-side-lengths of the box in each dimension.
*/
class Box : public SceneObject
{
public:
	Box(const Vector3& pos_, const Vector3& dim, const sf::Color& c, const bool neg = false) : SceneObject(pos_, c, neg), dimensions(dim) {}

	float distanceFromPoint(const Vector3& p) const
	{
		Vector3 value = Vector3::abs(p - pos) - dimensions;
		
		if (value.x < 0)
			value.x = 0;
		if (value.y < 0)
			value.y = 0;
		if (value.z < 0)
			value.z = 0;

		return negative ? -value.mag() : value.mag();
	}

protected:
	Vector3 dimensions;
};

// infinite flat plane (parallel to xz plane) with a given height (y position value)
class Plane : public SceneObject
{
public:
	Plane(const float height, const sf::Color& c, const bool neg = false) : SceneObject(Vector3(0, height, 0), c, neg) {}

	float distanceFromPoint(const Vector3& p) const
	{
		return negative ? (pos.y - p.y) : (p.y - pos.y);
	}
};

/*
* A mandelbulb is a 3D fractal similar the mandelbrot set. It has the additional parameters of iterations
* and power, which can be tweaked to create different variations of the fractal.
*/
class MandelBulb : public SceneObject
{
public:
	MandelBulb(const int iters_, const float power_, const sf::Color& c, const bool neg = false) : SceneObject(Vector3(), c, neg), iters(iters_), power(power_) {}

	float distanceFromPoint(const Vector3& p) const
	{
		Vector3 z = p;
		float dr = 1;
		float r = 0;
		for (int i = 0; i < iters; i++) {
			r = z.mag();
			if (r > 2)
				break;

			float phi = atan2(z.y, z.x) * power;
			float theta = acos(z.z / r) * power;
			float zr = pow(r, power);
			dr = pow(r, power - 1.0) * power * dr + 1.0;

			z = Vector3(sinf(theta) * cosf(phi), sinf(phi) * sinf(theta), cosf(theta)) * zr;
			z += p;
		}
		return 0.5 * log(r) * r / dr;
	}

	int iters;
	float power;
};