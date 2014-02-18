#include "classes.h"

using namespace std;

Coord::Coord() {
	this->x = 0;
	this->y = 0;
}

Coord::Coord(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

void Coord::set(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

Fractal::Fractal() {
	this->startingLength = 1;
	this->numIterations = 0;
	this->started = false;
}

void Fractal::setInitialLength(int l) {
	cout << "Setting start length to " << l << "." << endl;
	this->startingLength = l;
}

void Fractal::incrementIterations() {
	 this->numIterations++;
	 glutPostRedisplay();
}

void Fractal::draw() {
	cout << "Drawing the fractal." << endl;

	for (int i = 0; i < this->numIterations; i++) {
		this->recursiveDraw(i);
	}
}

void Fractal::recursiveDraw(int depth) {
	cout << "Recursing... " << depth << endl;
}

void Fractal::hide() {
	cout << "Hiding the fractal." << endl;
	glutPostRedisplay();
}

void Fractal::show() {
	cout << "Re-showing the fractal." << endl;
	glutPostRedisplay();
}

void Fractal::clear() {
	cout << "Clearing the fractal." << endl;
	glutPostRedisplay();
}

void Fractal::reset() {
	cout << "Resetting the fractal." << endl;
	glutPostRedisplay();
}

void Fractal::begin(int x, int y) {
	cout << "Starting the fractal." << endl;

	this->started = true;
	this->numIterations = 1;
	this->startingPos.set(x, y);

	glutPostRedisplay();
}