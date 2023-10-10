#include "Astroid.h"

Astroid::Astroid(Vector spawnLocation, Vector direction, float speed, float radius, bool clockwise, int sides, int rotationAngle, int rotationDelta) {
	this->spawnLocation = spawnLocation;
	this->direction = direction;
	this->speed = speed;
	this->radius = radius;
	this->clockwise = clockwise;
	this->sides = sides;
	this->rotationAngle = rotationAngle;
	this->rotationDelta = rotationDelta;
	this->hitpoints = 2;
}