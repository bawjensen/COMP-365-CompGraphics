#ifndef _CLASSES
#define _CLASSES

#include <GL/glut.h>
#include <iostream>

class Coord {
private:
	int x, y;
public:
	Coord();
	Coord(int, int);

	void set(int, int);
};

class SubFractal {
	
};

class Fractal {
private:
	Coord startingPos;
	int startingLength, numIterations;
	bool started;
public:
	Fractal();

	bool begun() { return this->started; };

	void setInitialLength(int);
	void incrementIterations();

	void draw();
	void recursiveDraw(int);
	void hide();
	void show();
	void clear();
	void reset();
	void begin(int, int);
};

#endif