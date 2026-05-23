#include "Vector3.h"

// normalizes the vector in place
void Vector3::normalize()
{
	float magnitude = mag();
	if (magnitude == 0)
		return;
	x = x / magnitude;
	y = y / magnitude;
	z = z / magnitude;
}

// returns this vector normalized
Vector3 Vector3::normalized() const
{
	float magnitude = mag();
	if (magnitude == 0)
		return *this;
	return Vector3(x / magnitude, y / magnitude, z / magnitude);
}

// component-wise addition
Vector3& Vector3::operator+=(const Vector3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

// component-wise assignment
Vector3& Vector3::operator=(const Vector3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

// returns distance between two 3D points
float Vector3::dist(const Vector3& a, const Vector3& b)
{
	Vector3 diff = b - a;
	return diff.mag();
}

// returns cross product of two vectors
Vector3 Vector3::cross(const Vector3& a, const Vector3& b)
{
	float x = a.y * b.z - a.z * b.y;
	float y = a.z * b.x - a.x * b.z;
	float z = a.x * b.y - a.y * b.x;
	return Vector3(x, y, z);
}