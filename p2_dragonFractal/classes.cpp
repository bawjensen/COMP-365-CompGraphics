#include "classes.h"

using namespace std;

static const int POINTS_LIMIT = 5000000; // Maximum number of points to display
static const int COLOR_SHIFT_SPEED = 5; // Rate at which the color will gradient from one to another
static const int COUNTER_CLOCKWISE = -1; // Constant for rotation direction
static const int CLOCKWISE = 1; // Constant for rotation direction
static const int RED = 0; // Constant for color indexing
static const int GREEN = 1; // Constant for color indexing
static const int BLUE = 2; // Constant for color indexing

// Default constructor
Coord::Coord() {
	this->x = 0;
	this->y = 0;
}

// Parameterized constructor
Coord::Coord(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

// Copy constructor
Coord::Coord(const Coord& other) {
	this->x = other.getX();
	this->y = other.getY();
}

// Setter for both x and y
void Coord::set(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

// Function to rotate the given point by degrees around the pivot.
Coord Coord::rotate(float degrees, Coord pivot) {
	float radians = degrees * M_PI / 180; // Convert to radians for cos and sin

	// Copy and shift
	float newX = this->x;
	float newY = this->y;

	newX -= pivot.getX();
	newY -= pivot.getY();

	// Rotate
	int rotatedX = round(newX * cos(radians) - newY * sin(radians));
	int rotatedY = round(newX * sin(radians) + newY * cos(radians));

	// Shift back
	rotatedX += pivot.getX();
	rotatedY += pivot.getY();


	return Coord(rotatedX, rotatedY);
}

// Equality operator
bool Coord::operator==(const Coord& other) {
	return (this->x == other.getX() && this->y == other.getY());
}

// Inequality operator (redirects to equality and returns the inverse)
bool Coord::operator!=(const Coord& other) {
	return ( !(*this == other) );
}

// stdio operator
ostream& operator<<(ostream& co, const Coord& point) {
	co << "(" << point.x << ", " << point.y << ")";
}

// Default constructor
Fractal::Fractal() {
	this->startingLength = 1;
	this->numIterations = 0;
	this->started = false;
	this->visible = true;
	this->direction = CLOCKWISE;
	this->startColor = RED;
}

// Setter for initial length
void Fractal::setInitialLength(int l) {
	cout << "Setting start length to " << l << "." << endl;
	this->startingLength = l;
}

// Setter for direction
void Fractal::setDirection(string newDirection) {
	if (newDirection == "cc") cout << "Setting direction of spiral to counter-clockwise." << endl;
	else cout << "Setting direction of spiral to clockwise." << endl;

	if (newDirection == "cc") this->direction = COUNTER_CLOCKWISE;
	else this->direction = CLOCKWISE;
}

// Setter for color - accepts string, stores as int constant-defined value
void Fractal::setColor(string newColor) {
	cout << "Setting new starting color to " << newColor << "." << endl;

	if (newColor == "red") this->startColor = RED;
	else if (newColor == "green") this->startColor = GREEN;
	else this->startColor = BLUE;

	glutPostRedisplay();
}

// Recreates the fractal with another iteration
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

// Draws the fractal to the screen
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

// Toggles the visibility of the fractal to off
void Fractal::hide() {
	cout << "Hiding the fractal." << endl;
	this->visible = false;
	glutPostRedisplay();
}

// Toggles the visibility of the fractal to on
void Fractal::show() {
	cout << "Re-showing the fractal." << endl;
	this->visible = true;
	glutPostRedisplay();
}

// Clears the contents of the fractal
void Fractal::clear() {
	cout << "Clearing the fractal." << endl;
	this->points.clear();
	glutPostRedisplay();
}

// Clears and resets the fractal
void Fractal::reset() {
	cout << "Resetting the fractal." << endl;
	this->clear();
	this->started = false;
	this->visible = true;
	glutPostRedisplay();
}

// Begins the fractal with a starter location
void Fractal::begin(int x, int y) {
	cout << "Initializing the fractal at (" << x << ", " << y << ")" << endl;

	this->started = true;
	this->numIterations = 1;

	this->points.push_back(Coord(x, y));

	x -= startingLength;

	this->points.push_back(Coord(x, y));
	this->pivotPoint.set(x, y);

	glutPostRedisplay();
}

// Fun function to create a cool pattern by alternating the spiral direction
void Fractal::alternate(int n) {
	if (!this->started) {
		cout << "ERROR: Fractal must be initialized with a click location before using the alternation pattern." << endl;
		cout << "Suggested usage: Initialize a point in the upper right corner, then activate the alternation pattern." << endl;
		return;
	}
	else {
		cout << "Creating alternation pattern..." << endl;
	}
	for (int i = 0; i < n; i++) {
		this->direction *= -1;

		this->incrementIterations();
	}

	glutPostRedisplay();
}