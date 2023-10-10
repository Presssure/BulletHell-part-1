#pragma once
#include "Vector.h"
class Astroid {
public:
	Vector spawnLocation;
	Vector direction;
	int speed;
	float radius;
	bool clockwise;
	int sides;
	int rotationAngle;
	int rotationDelta;
	int hitpoints;
	Astroid(Vector spawnLocation,Vector direction, float speed,  float radius, bool clockwise, int sides, int rotationAngle, int rotationDelta);
};