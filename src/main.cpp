#pragma once
#ifndef MAIN
#define MAIN
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Arena.h"
#include "Ship.h"
#include "Keyboard.h"
#include "Astroid.h"
#include <vector>
#include "Bullet.h"

#endif // !MAINW
#if _WIN32
#   include <Windows.h>
#endif
#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif
#include "Hashdefines.cpp"

int g_screen_width = 0;
int g_screen_height = 0;
//Window window;
float g_last_time = 0.0;
float g_astroid_time = 0.0;
Arena arena;
Ship g_ship;
Keyboard keyboard;
int g_waves = 0;
int g_tempTime = 0;
int g_points = 0;
int g_gameTime = 0;
float shootTime = 0;
bool g_spawnBullet = false;

std::vector<Astroid> astroidVector;
std::vector <Vector> locations;
std::vector<Bullet> bulletVector;
void on_keypress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
	case 'q':
		exit(EXIT_SUCCESS);
		break;
	case MOVEFORWARD:
		keyboard.input = MOVEFORWARD;
		break;
	case ROTATELEFT:
		keyboard.input = ROTATELEFT;
		break;
	case ROTATERIGHT:
		keyboard.input = ROTATERIGHT;
		break;
	default:
		break;
	}
}
void on_mousepress(int key, int state, int x, int y) {
	switch (state) {
	case(GLUT_DOWN):
		g_spawnBullet = true;
		
		break;
	}
}

void initShip(Ship* ship) {
	ship->position.x = g_screen_width * 0.2;
	ship->position.y = g_screen_height * 0.2;
	ship->angle = 0;
	ship->direction.x = 0;
	ship->direction.y = 1;
	ship->alive = true;
	ship->radius = 20;
}



void moveShip(Ship* ship, Keyboard* keyboard) {
	if (!ship->alive) {
		initShip(&g_ship);
	}
	if (keyboard->input == MOVEFORWARD) {
		Vector velocity;
		velocity.x = 10;
		velocity.y = 10;
		ship->position = scalarMultiply(ship->direction, ship->position, velocity);
		keyboard->input = ' ';
		//std::cout << "Danger Zone:  "<< g_screen_width * DANGERZONE << "Danger Zone lower:  " << g_screen_width * (100-DANGERZONE) << "  Ship position x:  " << ship->position.x << std::endl;
		//std::cout << "Danger Zone:  " << g_screen_height * DANGERZONE << "Danger Zone lower:  " << g_screen_height * (100 - DANGERZONE)  << "  Ship position y:  " << ship->position.y << std::endl;
		//std::cout << "Arena Zone:  "<< g_screen_width * ARENASIZE * 0.01 << "   Arena Zone lower:  " << g_screen_width * (100- ARENASIZE) *0.01<< "  Ship position x:  " << ship->position.x << std::endl;
		//std::cout << "Arena Zone:  " << g_screen_height * ARENASIZE * 0.01 << "   Arena Zone lower:  " << g_screen_height * (100 - ARENASIZE) * 0.01 << "  Ship position y:  " << ship->position.y << std::endl;
		//std::cout << "ship position   "<<ship->position.x + ship->radius << "   arena width:  " <<arena.width << std::endl;
	}
	else if (keyboard->input == ROTATERIGHT) {
		ship->angle = (ship->angle + ANGLE)%360;
		ship->direction = calcVector(ship->angle);
		keyboard->input = ' ';
	}
	else if (keyboard->input == ROTATELEFT) {
		ship->angle = (ship->angle - ANGLE)%360;
		ship->direction = calcVector(ship->angle);
		keyboard->input = ' ';
	}

}



void checkDanger(Ship *ship, Arena* arena) {
	if (ship->position.x >= g_screen_width * DANGERZONE*0.01) {
		arena->danger = true;
	}
	else if(ship->position.y >= g_screen_height * DANGERZONE*0.01) {
		arena->danger = true;
	}
	else if(ship->position.x <= g_screen_width * (100-DANGERZONE)*0.01) {
		arena->danger = true;
	}
	else if(ship->position.y <= g_screen_width * (100-DANGERZONE)*0.01) {
		arena->danger = true;
	}
	else {
		arena->danger = false;
	}
}

void checkBullet(Bullet* bullet) {
	std::cout << "xPos:  " << bullet->position.x << "  yPos:  " << bullet->position.y << std::endl;
	if (bullet->position.x + (BULLETSIZE * 2) >= arena.width) {
		bullet->alive = false;
	}
	else if (bullet->position.y + (BULLETSIZE * 2) >= arena.height) {
		bullet->alive = false;
	}
	else if (bullet->position.x + (BULLETSIZE * 2) <= g_screen_width * (100 - ARENASIZE) * 0.01) {
		bullet->alive = false;
	}
	else if (bullet->position.y + (BULLETSIZE * 2) <= g_screen_height * (100 - ARENASIZE) * 0.01) {
		bullet->alive = false;
	}
	else {
		bullet->alive = true;
	}
}
void checkBulletAsteroid(std::vector <Astroid> *astroids, Bullet *bullet) {
	for (int i = 0; i < astroids->size();i++) {
		if (findCollision(astroids->at(i).spawnLocation, bullet->position,astroids->at(i).radius,BULLETSIZE)) {
			bullet->alive = false;
			astroids->at(i).hitpoints--;
			if (astroids->at(i).hitpoints < 1) {
				astroids->erase(astroids->begin() + i);
				g_points++;
			}
		}
	}
}

void checkAlive(Ship* ship) {

	//if (ship->position.x+ship->radius >= g_screen_width * ARENASIZE * 0.01) {
	if (ship->position.x +ship->radius >= arena.width) {
		ship->alive = false;
		std::cout << "dead" << std::endl;
	}
	else if (ship->position.y + ship->radius >= arena.height) {
		ship->alive = false;
		std::cout << "dead" << std::endl;
	}
	else if (ship->position.x - ship->radius <= g_screen_width * (100 - ARENASIZE) * 0.01) {
		ship->alive = false;
		std::cout << "dead" << std::endl;
	}
	else if (ship->position.y - ship->radius <= g_screen_height * (100 - ARENASIZE) * 0.01) {
		ship->alive = false;
		std::cout << "dead" << std::endl;
	}
	else {
		ship->alive = true;
	}
}
bool checkAstroid(Astroid* astroid) {
	bool answer = true;
		//if (ship->position.x+ship->radius >= g_screen_width * ARENASIZE * 0.01) {
		if (astroid->spawnLocation.x + astroid->radius >= arena.width+100) {
			answer = false;
		}
		else if (astroid->spawnLocation.y + astroid->radius >= arena.height+100) {
			answer = false;
		}
		else if (astroid->spawnLocation.x - astroid->radius <= 0-100 ){
			answer = false;
		}
		else if (astroid->spawnLocation.y - astroid->radius <= 0-100 ){
			answer = false;
		}
		return answer;
}

void drawBullet(Bullet* bullet) {
	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(bullet->position.x, bullet->position.y, 0.0);
	glRotatef(-bullet->angle, 0, 0, 1);
	glScalef(BULLETSIZE, BULLETSIZE, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.2,1);
	glVertex2f(-0.2, 2);
	glVertex2f(0.2, 2);
	glVertex2f(0.2,1);
	glVertex2f(-0.2,1);
	glEnd();
	glPopMatrix();
}
void makeCircle(int radius) {

	for (double i = 0; i < 2 * 3.14159;i += 3.14159 / radius) {
		glVertex2f(cos(i) * radius, sin(i) * radius);
	}
	return;
}
void drawCircle(int sides, float radius) {
	for (int a = 0; a < 360; a += 360 / sides) {
		float heading = a * 3.14159 / 180;
		glVertex2f(cos(heading) * radius, sin(heading) * radius);
	}
}
std::vector<Vector> circleVector(int radius) {
	std::vector <Vector> values;
	for (double i = 0; i < 2 * 3.14159;i += 3.14159 / radius) {
		glVertex2f(cos(i) * radius, sin(i) * radius);
		Vector temp;
		temp.x = (cos(i) * radius)+ g_screen_width / 2;
		temp.y= (sin(i) * radius)+ g_screen_height / 2;
		values.push_back(temp);
	}
	return values;
}
void drawAstroidbelt(Ship* ship) {
	//std::vector <float> heading = makeCircle(20);
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslatef(g_screen_width/2, g_screen_height/2, 0.0);
	glBegin(GL_LINE_LOOP);
	makeCircle(ASTROIDBELTRADIUS);
	glEnd();
	glPopMatrix();
}

void drawAstroid( Ship *ship, Astroid* astroid) {
	int tempSpin = -1;
	if (astroid->clockwise) {
		tempSpin = 1;
	}
	std::cout << tempSpin << std::endl;
	glPushMatrix();
	//glTranslatef(g_screen_width / 2, g_screen_height / 2, 0.0);
	glColor3f(0.5, 0, 0.5);
	glTranslatef(astroid->spawnLocation.x, astroid->spawnLocation.y, 0.0);
	glRotatef(astroid->rotationAngle*tempSpin, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	//makeCircle(astroid->radius);
	
	drawCircle(astroid->sides, astroid->radius);
	glEnd();
	glPopMatrix();
}
void drawCircle(Ship *ship) {
	
	//std::vector <float> heading = makeCircle(20);
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslatef(ship->position.x, ship->position.y, 0.0);
	glBegin(GL_LINE_LOOP);
	makeCircle(ship->radius);
	glEnd();
	glPopMatrix();
}

void drawShip(Ship* ship) {
	glPushMatrix();
	glColor3f(SHIP_FILL);
	glTranslatef(ship->position.x, ship->position.y, 0.0);
	glScalef(20, 20, 0.0);
	glRotatef(-ship->angle, 0, 0, 1);
	glBegin(GL_POLYGON);
	glVertex2f(0, 1);
	glVertex2f(0, -0.7);
	glVertex2f(-1, -1);
	glVertex2f(0, 1);
	glVertex2f(1, -1);
	glVertex2f(0, -0.7);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glColor3f(SHIP_OUTLINE);
	glTranslatef(ship->position.x, ship->position.y, 0.0);
	glScalef(20, 20, 0.0);
	glRotatef(-ship->angle, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0, 1);
	glVertex2f(0, -0.7);
	glVertex2f(-1, -1);
	glVertex2f(0, 1);
	glVertex2f(1, -1);
	glVertex2f(0, -0.7);
	glEnd();
	glPopMatrix();

}

void initArena(float r, float g, float b, Arena* arena, int g_screen_width, int g_screen_height) {
	arena->size = 500;
	arena->r = r;
	arena->g = g;
	arena->b = b;
	arena->xPos = g_screen_width / 2;
	arena->yPos = g_screen_height / 2;
	std::cout <<  g_screen_width / 2 << std::endl;
	std::cout << g_screen_height / 2 << std::endl;
	arena->height = g_screen_height * ARENASIZE * 0.01;
	arena->width = g_screen_width * ARENASIZE * 0.01;
	arena->danger = false;
}

void on_reshape(int w, int h) {
	glViewport(0, 0, w, h);
	g_screen_width = w;
	g_screen_height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

	initArena(0, 1, 0, &arena, g_screen_width, g_screen_height);
	initShip(&g_ship);
	locations = circleVector(ASTROIDBELTRADIUS);
	g_tempTime= glutGet(GLUT_ELAPSED_TIME) / 1000.0+3;
}

void updateAstroid(std::vector<Astroid> *astroids, float dt) {
	for (int i = 0; i < astroids->size(); i++) {
		Vector tempVector;
		tempVector.x = astroids->at(i).speed * dt;
		tempVector.y = astroids->at(i).speed * dt;
		
		astroids->at(i).spawnLocation = scalarMultiply(astroids->at(i).direction, astroids->at(i).spawnLocation, tempVector);
		astroids->at(i).rotationAngle += astroids->at(i).rotationDelta * dt;
		//std::cout <<i<< "   xPos:  " << astroids->at(i).spawnLocation.x << "   yPos:  " << astroids->at(i).spawnLocation.y << std::endl;
		if (findCollision(astroids->at(i).spawnLocation, g_ship.position, astroids->at(i).radius, g_ship.radius)) {
			g_ship.alive = false;
			std::cout << "ship died" << std::endl;
		}
		if (!checkAstroid(&astroids->at(i))) {
			astroids->erase(astroids->begin()+i);
		}
		
	}
}

void updateBullet(std::vector<Bullet>* bullet, float dt) {
	for (int i = 0; i < bullet->size(); i++) {
		Vector tempVector;
		tempVector.x = bullet->at(i).speed * dt;
		tempVector.y = bullet->at(i).speed * dt;
		checkBullet(&bullet->at(i));
		bullet->at(i).position = scalarMultiply(bullet->at(i).direction, bullet->at(i).position, tempVector);
		checkBulletAsteroid(&astroidVector, &bullet->at(i));
		if (!bullet->at(i).alive) {
			bullet->erase(bullet->begin() + i);
		}
		//std::cout << bullet->at(i).position.x <<"-"<< bullet->at(i).position.y <<std::endl;
		//std::cout <<i<< "   xPos:  " << astroids->at(i).spawnLocation.x << "   yPos:  " << astroids->at(i).spawnLocation.y << std::endl;
		/*if (findCollision(astroids->at(i).spawnLocation, g_ship.position, astroids->at(i).radius, g_ship.radius)) {
			g_ship.alive = false;
			std::cout << "ship died" << std::endl;
		}
		if (!checkAstroid(&astroids->at(i))) {
			astroids->erase(astroids->begin() + i);
		}*/
	}
}


void drawArena(Arena* arena) {
	if (arena->danger) {
		arena->r = 1;
		arena->g = 0;
	}
	else {
		arena->r = 0;
		arena->g = 1;
	}
	//std::cout <<"arena position:  "<< arena->xPos * ARENASIZE * 0.01 << std::endl;
	glPushMatrix();
	glColor3f(arena->r, arena->g, arena->b);
	glTranslatef(arena->xPos, arena->yPos, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(-arena->xPos * 90*0.01, -arena->yPos * 90 * 0.01, 0);
	glVertex3f(arena->xPos * 90 * 0.01, -arena->yPos * 90 * 0.01, 0);
	glVertex3f(arena->xPos * 90 * 0.01, arena->yPos * 90 * 0.01, 0);
	glVertex3f(-arena->xPos * 90 * 0.01, arena->yPos * 90 * 0.01, 0);
	glEnd();
	glPopMatrix();

}
void printText(int x, int y, float r, float g, float b, char* string)
{
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	int len, i;
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}
void printTime(bool left) {

	char time[50];

	int timeText = g_gameTime;
	int textWidth = g_screen_width * ARENASIZE * 0.01;
	if(left){
		textWidth = g_screen_width * (100 - ARENASIZE) * 0.01;
		sprintf_s(time, "%d", timeText);
	}
	else {
		timeText = g_points;
		sprintf_s(time, "%d", timeText);
	}
	//std::string temp = std::to_string(g_gameTime);
	

	
	int textHeight = g_screen_height * ARENASIZE * 0.01;
	printText(textWidth, textHeight, 1, 1, 1, time);
}

void render_frame() {

	drawArena(&arena);
	drawShip(&g_ship);
	//drawCircle(&g_ship);
	//drawAstroidbelt(&g_ship);
	//std::cout << "ASTROID SIZE:   " << astroidVector.size() << std::endl;
	for (int i = 0; i < astroidVector.size(); i++) {

		drawAstroid(&g_ship, &astroidVector[i]);
		//std::cout << "x Spawn:  " << astroidVector[i].spawnLocation.x << "   y spawn:  " << astroidVector[i].spawnLocation.x << std::endl;
	}
	for (int i = 0; i < bulletVector.size(); i++) {
		drawBullet(&bulletVector[i]);
		//std::cout << "drawing bullet" << std::endl;
	}
	printTime(true);
	printTime(false);
	

}
void spawnAstroid() {
	//std::cout << "ASTROID SPAWNED" << std::endl;

	int randVec = rand() % locations.size();
	//std::cout << rand() % locations.size() << std::endl;
	Vector tempLoc = locations[randVec];
	Vector tempPos;
	tempPos.x = g_ship.position.x - tempLoc.x;
	tempPos.y = g_ship.position.y - tempLoc.y;
	float tempLength = lengthVector(tempPos);
	Vector tempUnit = unitVector(tempPos, tempLength);
	float tempSpeed = MINASTROIDSPEED + rand() % (MINASTROIDSPEED - MAXASTROIDSPEED);
	int randSpin = rand()%2;
	int randSides = (rand() % 15)+6;
	int randAngle = rand() % 360;
	int randDelta = ASTEROID_ROATION_DELTA_MIN + rand() % (ASTEROID_ROATION_DELTA_MIN- ASTEROID_ROATION_DELTA_MAX);
	Astroid* astroid = new Astroid(tempLoc, tempUnit, tempSpeed, 50, randSpin, randSides, randAngle, randDelta);
	/*std::cout << "direction x:  " << astroid->direction.x << "direction y:  " << astroid->direction.y << " location x:  " << astroid->spawnLocation.x <<
		" location y:  " << astroid->spawnLocation.y << "  ship x:  " << g_ship.position.x << "  ship y:  " << g_ship.position.y << std::endl;*/
	astroidVector.push_back(*astroid);
}

void on_idle()
{
	g_gameTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	float cur_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	/*if (g_astroid_time <= glutGet(GLUT_ELAPSED_TIME) / 1000.0) {
		g_astroid_time = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) + 2;
		spawnAstroid = true;
	}*/
	if (g_spawnBullet) {
		if (shootTime <= cur_time) {
			shootTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0 + BULLETFIRERATE;
			Bullet* bullet = new Bullet(true, BULLETSPEED, g_ship.direction, g_ship.position, g_ship.angle);
			bulletVector.push_back(*bullet);
			std::cout << "clicked mouse " << std::endl;
		}
		g_spawnBullet = false;
	}
	int tempWave = glutGet(GLUT_ELAPSED_TIME);
		//std::cout << tempWave % ASTEROIDWAVE << std::endl;
		if (tempWave % ASTEROIDWAVE == 0) {
			g_waves++;
			std::cout << "wave:  " << g_waves << std::endl;
			for (int i = 0; i < g_waves; i++) {
				spawnAstroid();
			}
		}
	//std::cout << "astroid time:  " << g_astroid_time << "   dt=  " << glutGet(GLUT_ELAPSED_TIME) / 1000.0 << std::endl;
		
	
	float dt = cur_time - g_last_time;
	updateAstroid(&astroidVector,  dt);
	updateBullet(&bulletVector, dt);
	//std::cout << glutGet(GLUT_ELAPSED_TIME)/1000 << std::endl;
	

	g_last_time = cur_time;
	moveShip(&g_ship, &keyboard);
	checkDanger(&g_ship, &arena);
	checkAlive(&g_ship);
	glutPostRedisplay();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	render_frame();


	//Enable(GL_DEPTH_TEST);

	/* Always check for errors! */
	//std::cout << "Hello" << std::endl;
	int err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("display:  %s\n", gluErrorString(err));
		//std::cout << "display:" << gluErrorString(err) << std::endl;
	}


	glutSwapBuffers();

}
void initTime() {
	g_last_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}



void init_app(int* argcp, char** argv) {
	glutInit(argcp, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("A1-Last");
	//init();
	//glutFullScreen();
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keypress);
	glutMouseFunc(on_mousepress);
	glutDisplayFunc(display);
	glutIdleFunc(on_idle);

	/*for (int i = 0; i < NUM_OBJECTS; i++) {
		game_object_init(&squareArray[i], 100, 300);
	}*/
	initTime();
}




int main(int argc, char** argv) {
	init_app(&argc, argv);
	glutMainLoop();
	return(EXIT_SUCCESS);
}