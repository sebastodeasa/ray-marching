#pragma once
#include <vector>
#include "Vector3.h"
#include "SceneObjects.h"
class Scene
{
public:
	float minDistanceFromPoint(const Vector3& p) const; // returns minimum distance from point p to any object in the scene
	float maxDistanceFromPoint(const Vector3& p) const; // returns maximum distance from point p to any object in the scene
	sf::Color closestColorToPoint(const Vector3& p) const; // returns color of object closest to point p
	sf::Color furthestColorFromPoint(const Vector3& p) const; // returns color of object furthest from point p
	void addObject(SceneObject* const obj) { objects.push_back(obj); }

	~Scene();

private:
	void clearObjects(); // deletes all objects in the scene and clears the vector (called by destructor)

	std::vector<SceneObject*> objects; // list of (pointers to) all objects in the scene
};