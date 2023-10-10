#include "Bullet.h"
Bullet::Bullet(bool alive, int speed, Vector direction, Vector position, int angle) {
	this->alive = alive;
	this->speed = speed;
	this->direction = direction;
	this->position = position;
	this->angle = angle;
}