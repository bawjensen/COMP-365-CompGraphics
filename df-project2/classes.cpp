#include "classes.h"

using namespace std;

static const int POINTS_LIMIT = 5000000;
static const int COLOR_SHIFT_SPEED = 5;
static const int COUNTER_CLOCKWISE = -1;
static const int CLOCKWISE = 1;
static const int RED = 0;
static const int GREEN = 1;
static const int BLUE = 2;

Coord::Coord() {
	this->x = 0;
	this->y = 0;
}

Coord::Coord(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

Coord::Coord(const Coord& other) {
	this->x = other.getX();
	this->y = other.getY();
}

void Coord::set(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

void Coord::shift(int shiftAmount) {
	this->y += shiftAmount;
}

Coord Coord::rotate(float degrees, Coord pivot) {
	// cout << "Rotating " << *this << " " << degrees << " degrees around " << pivot << ": ";
	float radians = degrees * M_PI / 180;

	float newX = this->x;
	float newY = this->y;

	// cout << "Created: (" << newX << ", " << newY << ")" << endl;

	newX -= pivot.getX();
	newY -= pivot.getY();

	// cout << "Shifted to origin: (" << newX << ", " << newY << ")" << endl;

	int rotatedX = round(newX * cos(radians) - newY * sin(radians));
	int rotatedY = round(newX * sin(radians) + newY * cos(radians));

	// cout << "Rotated: (" << newX << ", " << newY << ")" << endl;

	rotatedX += pivot.getX();
	rotatedY += pivot.getY();

	return Coord(rotatedX, rotatedY);
}

bool Coord::operator==(const Coord& other) {
	return (this->x == other.getX() && this->y == other.getY());
}

bool Coord::operator!=(const Coord& other) {
	return ( !(*this == other) );
}

ostream& operator<<(ostream& co, const Coord& point) {
	co << "(" << point.x << ", " << point.y << ")";
}

Fractal::Fractal() {
	this->startingLength = 1;
	this->numIterations = 0;
	this->started = false;
	this->visible = true;
	this->direction = CLOCKWISE;
	this->startColor = RED;
}

void Fractal::setInitialLength(int l) {
	cout << "Setting start length to " << l << "." << endl;
	this->startingLength = l;
}

void Fractal::setDirection(string newDirection) {
	cout << "Setting direction of spiral: " << newDirection << endl;

	if (newDirection == "cc") this->direction = COUNTER_CLOCKWISE;
	else this->direction = CLOCKWISE;
}

void Fractal::setColor(string newColor) {
	cout << "Setting new starting color: " << newColor << endl;

	if (newColor == "red") this->startColor = RED;
	if (newColor == "green") this->startColor = GREEN;
	else this->startColor = BLUE;
}

void Fractal::incrementIterations() {
	vector<Coord> tempVect;

	if (this->points.size() > POINTS_LIMIT) {
		cout << "Too many points to show..." << endl;
		return;
	}

	for (vector<Coord>::iterator it = this->points.begin(); it != this->points.end(); ++it) {
		if ( *it != this->pivotPoint ) {
			tempVect.push_back(it->rotate(this->direction * 90, this->pivotPoint));
		}
	}

	for (vector<Coord>::reverse_iterator it = tempVect.rbegin(); it != tempVect.rend(); ++it) {
		this->points.push_back(*it);
	}

	this->pivotPoint = this->points.back();

	glutPostRedisplay();
}

void Fractal::draw() {
	if (!this->visible || !this->started) return;

	float colorIncrementers[3];
	colorIncrementers[RED] = (float)1 / POINTS_LIMIT * COLOR_SHIFT_SPEED;
	colorIncrementers[GREEN] = (float)1 / POINTS_LIMIT * COLOR_SHIFT_SPEED;
	colorIncrementers[BLUE] = (float)1 / POINTS_LIMIT * COLOR_SHIFT_SPEED;

	float colors[3] = { 0, 0, 0 };

	colors[this->startColor] = 1;
	colorIncrementers[this->startColor] *= -1;

	glBegin(GL_LINE_STRIP);

	for (vector<Coord>::iterator it = this->points.begin(); it != this->points.end(); ++it) {
		colors[RED] += colorIncrementers[RED];
		colors[GREEN] += colorIncrementers[GREEN];
		colors[BLUE] += colorIncrementers[BLUE];

		glColor3f( colors[0], colors[1], colors[2] );
		glVertex2i(it->getX(), it->getY());
	}

	glEnd();
}

void Fractal::hide() {
	cout << "Hiding the fractal." << endl;
	this->visible = false;
	glutPostRedisplay();
}

void Fractal::show() {
	cout << "Re-showing the fractal." << endl;
	this->visible = true;
	glutPostRedisplay();
}

void Fractal::clear() {
	cout << "Clearing the fractal." << endl;
	this->points.clear();
	glutPostRedisplay();
}

void Fractal::reset() {
	cout << "Resetting the fractal." << endl;
	this->clear();
	this->started = false;
	this->visible = true;
	glutPostRedisplay();
}

void Fractal::begin(int x, int y) {
	cout << "Starting the fractal at (" << x << ", " << y << ")" << endl;

	this->started = true;
	this->numIterations = 1;

	this->startingPos.set(x, y);
	this->points.push_back(Coord(x, y));

	x -= startingLength;

	this->points.push_back(Coord(x, y));
	this->pivotPoint.set(x, y);

	glutPostRedisplay();
}

void Fractal::alternate(int n) {
	if (!this->started) {
		cout << "Fractal must be initialized with a click location before using the alternation pattern." << endl;
		cout << "Suggested usage: Initialize a point in the upper right corner, then activate the alternation pattern." << endl;
		return;
	}
	for (int i = 0; i < n; i++) {
		this->direction *= -1;

		this->incrementIterations();
	}
}