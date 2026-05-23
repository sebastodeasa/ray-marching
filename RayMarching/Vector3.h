#pragma once
#include <cmath>

// simple 3D vector class with some helpful functions for ray marching
class Vector3
{
public:
	Vector3() : x(0), y(0), z(0) {}
	Vector3(const float x_, const float y_, const float z_) : x(x_), y(y_), z(z_) {}

	float mag() const { return sqrt(x * x + y * y + z * z); } // returns magnitude of the vector
	void normalize(); // normalizes the vector in place
	Vector3 normalized() const; // returns this vector normalized

	static Vector3 abs(const Vector3& a) { return Vector3(std::abs(a.x), std::abs(a.y), std::abs(a.z)); } // performs component-wise absolute value
	static float dist(const Vector3& a, const Vector3& b); // returns distance between two 3D points
	static float dot(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; } // dot product
	static Vector3 cross(const Vector3& a, const Vector3& b); // cross product

	Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); } // component-wise subtraction
	Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); } // component-wise addition
	Vector3 operator*(const float rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); } // component-wise scalar multiplication
	Vector3& operator+=(const Vector3& rhs);
	Vector3& operator=(const Vector3& rhs);

	float x;
	float y;
	float z;
};