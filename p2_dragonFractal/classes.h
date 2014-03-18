#ifndef _CLASSES
#define _CLASSES

#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif
	
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES // Gets the M_PI constant from cmath
#include <cmath>

using namespace std;

class Coord {
private:
	int x, y;
public:
	Coord();
	Coord(int, int);
	Coord(const Coord&);

	int getX() const { return this->x; };
	int getY() const { return this->y; };

	void set(int, int);
	Coord rotate(float, Coord);

	bool operator==(const Coord&);
	bool operator!=(const Coord&);
	friend ostream& operator<<(ostream&, const Coord&);
};

class Fractal {
private:
	Coord pivotPoint;
	int startingLength, numIterations, direction, startColor;
	bool started, visible;
	vector<Coord> points;
public:
	Fractal();

	bool begun() { return this->started; };

	void setInitialLength(int);
	void setDirection(string);
	void setColor(string);
	void incrementIterations();

	void draw();
	void recursiveDraw(int);
	void hide();
	void show();
	void clear();
	void reset();
	void begin(int, int);
	void alternate(int);
};

#endif