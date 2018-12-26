// Clock_OpenGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <vector>

#pragma warning(disable:4996)

using namespace std;

bool FULL_SCREEN = false;
const double PI = 3.14159265358979323;
double rotation = PI / 2;

void 
keyboardCB(unsigned char, int, int);

void 
displayCB();

void
createClock();

void
updateTime();

void
reshapeCB(int,int);

struct Clock {
	struct Point;
	vector<Point> arrows;
	vector<Point> circle;
	vector<Point> dots;
} mainClock;

struct Clock::Point {
	double x, y;
	Point(double x_, double y_) {
		x = x_;
		y = y_;
	}
};


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);

	glutCreateWindow("Clock");

	createClock();

	glutKeyboardFunc(keyboardCB);
	glutReshapeFunc(reshapeCB);
	glutDisplayFunc(displayCB);

	glutMainLoop();

	return 0;
}

void keyboardCB(unsigned char key, int x, int y) {
	if (key == 'q') {
		exit(0);
	}
	else if (key == 'f') {
		if (FULL_SCREEN) {
			int w = 500, h = 500;
			glutReshapeWindow(w, h);
			FULL_SCREEN = false;
		}
		else {
			glutFullScreen();
			FULL_SCREEN = true;
		}
	}
}

void displayCB(void) {	
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glLineWidth(3.0);

	//dots around and center
	glPointSize(10.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (auto point : mainClock.dots) {
		glVertex2f(point.x, point.y);
	}
	glEnd();

	//circle
	glBegin(GL_LINE_LOOP);
	for (auto point : mainClock.circle) {
		glVertex2f(point.x, point.y);
	}
	glEnd();

	updateTime();

	//arrows
	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0.0);
	//for hours arrow
	for (size_t i = 0; i < mainClock.arrows.size(); i++) {
		if (i == 4) {
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
		}
		glVertex2f(mainClock.arrows[i].x, mainClock.arrows[i].y);
	}
	glEnd();
	glFlush();
	glutPostRedisplay();
}

void createClock() {
	const size_t resolution = 1000;
	const double step = 2 * PI / resolution;	
	double x, y;

	//dots around and center
	for (int i = 0; i < 12; i++) {
		mainClock.dots.push_back(Clock::Point(0.95 * cos(i * 2 * PI / 12.0), 0.95 * sin(i * 2 * PI / 12.0)));
	}
	mainClock.dots.push_back(Clock::Point(0.0, 0.0));

	//circle
	for (size_t i = 0; i < resolution; i++) {
		x = 0.95 * cos(i * step);
		y = 0.95 * sin(i * step);

		mainClock.circle.push_back(Clock::Point(x, y));
	}
}

void updateTime() {
	time_t t = time(0);
	tm* now = localtime(&t);
	int hours = now->tm_hour % 12;
	int minutes = now->tm_min;
	int seconds = now->tm_sec;
	double hrs = double(hours) / 12;
	double mins = double(minutes) / 60;
	double secs = double(seconds) / 60;
	double minsAngle = -2 * PI * mins + rotation;
	double hrsAngle = -2 * PI * hrs + rotation - (PI / 6) * mins;
	double secsAngle = -2 * PI * secs + rotation;

	mainClock.arrows.erase(mainClock.arrows.begin(), mainClock.arrows.end());

	//arrows
	//-------hours
	mainClock.arrows.push_back(Clock::Point(0.0, 0.0));
	mainClock.arrows.push_back(Clock::Point(0.6 * cos(hrsAngle), 0.6 * sin(hrsAngle)));
	//-------minutes
	mainClock.arrows.push_back(Clock::Point(0.0, 0.0));
	mainClock.arrows.push_back(Clock::Point(0.9 * cos(minsAngle), 0.9 * sin(minsAngle)));
	//-------seconds
	mainClock.arrows.push_back(Clock::Point(0.0, 0.0));
	mainClock.arrows.push_back(Clock::Point(0.9 * cos(secsAngle), 0.9 * sin(secsAngle)));
}

void reshapeCB(int w, int h) {
	float r = float(w) / h;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.5 * r, 1.5 * r, -1.5, 1.5, 0, 1);
	glMatrixMode(GL_MODELVIEW);
}