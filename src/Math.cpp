#include "Math.h"
Vector scalarMultiply(Vector direction, Vector position, Vector velocity) {
	float x = direction.x * velocity.x;
	float y = direction.y * velocity.y;
	x += position.x;
	y += position.y;
	Vector temp;
	temp.x = x;
	temp.y = y;
	return temp;
}

Vector calcVector(float angle)
{
	Vector retVal;
	retVal.y=cos(degToRadians(angle));
	retVal.x=sin(degToRadians(angle));
	std::cout << "This is the angle: " << angle << "x calc:=" << cos(degToRadians(angle)) << "y calc:= " << sin(degToRadians(angle)) << std::endl;
	return retVal;
}

float degToRadians(float angle) {
	float retVal = angle * 3.14159 / 180.0;
	return retVal;
}


Vector unitVector(Vector vector, float length) {
	Vector normalised;
	normalised.x= vector.x / length;
	normalised.y= vector.y / length;
	return normalised;
}

float lengthVector(Vector vector) {
	float sqX = vector.x * vector.x;
	float sqY = vector.y * vector.y;
	float total = sqX + sqY;
	return sqrt(total);
}

bool findCollision(Vector v1, Vector v2, float radius1, float radius2) {
	bool collision = false;
	float distX = v1.x - v2.x;
	float distY = v1.y - v2.y;
	float distance = sqrt((distX * distX) + (distY * distY));
	//std::cout << distance << " <= " << radius1 + radius2 << std::endl;
	if (distance <= radius1 + radius2) {
		collision = true;
	}
	return collision;
}