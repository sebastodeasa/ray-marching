#pragma once
#include "Vector3.h"
#include "Scene.h"

// struct to hold camera settings that can be easily tweaked
struct CameraSettings
{
	float normalFov = 1;
	float zoomFov = 0.2;
	float renderDist = 50;
	int maxSteps = 150;
	float minDist = 0.002;
	float moveSpeed = 0.05;
	float lookSpeed = 0.002;
};

class Camera
{
public:
	Camera(Scene* s, const Vector3& p = Vector3()) : activeScene(s), pos(p) {}

	
	sf::Color marchRay(const float x, const float y) const; // marches ray. x and y params are in NDC space (0 = top/left, 1 = bottom/right)
	void move(const sf::Vector3i& moveInput); // moves camera based on input vector
	void look(const sf::Vector2i& mouseInput); // rotates camera based on mouse input vector

	// position and orientation variables
	Vector3 pos; // position vector in world space
	Vector3 forward = Vector3(1.0f, 0.0f, 0.0f); // camera's forward vector in world space
	Vector3 up =      Vector3(0.0f, 1.0f, 0.0f); // upwards
	Vector3 right =   Vector3(0.0f, 0.0f, 1.0f); // to the right

	// camera yaw and pitch in radians
	float yaw = 0;
	float pitch = 0;

	CameraSettings settings;

	Scene* activeScene; // scene that the camera is currently rendering
	bool findsMinDistance = true; // whether ray marcher should find minimum distance to objects (true for normal ray marching, false for boolean rendering)
	float distFunc(const Vector3& p) const { return findsMinDistance ? activeScene->minDistanceFromPoint(p) : activeScene->maxDistanceFromPoint(p); }
	sf::Color colorAtPoint(const Vector3& p) const { return findsMinDistance ? activeScene->closestColorToPoint(p) : activeScene->furthestColorFromPoint(p); }

	bool calculateLighting = true; // if true, objects are shaded normally. if false, each pixel is shaded based on how many steps the corresponding ray took
	bool isZoomed = false; // whether camera should use normal or zoom FOV

private:
	
	Vector3 ndcToRay(const float x, const float y) const; // creates a vector in world space from a pixel in NDC space
};