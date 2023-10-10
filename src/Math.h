#pragma once
#ifndef MATH_H

#include <iostream>
#include <math.h>
#include "Vector.h"

Vector scalarMultiply(Vector direction, Vector position, Vector velocity);
Vector calcVector(float angle);
float degToRadians(float angle);
float lengthVector(Vector vector);
Vector unitVector(Vector vector, float length);
bool findCollision(Vector v1, Vector v2, float radius1, float radius2);
#endif // !MATH_H