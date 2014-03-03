#ifndef _CLASSES
#define _CLASSES

#define _USE_MATH_DEFINES // Needed to get MATH_PI from cmath
#include <cmath>
#include <iostream>

using namespace std;

class Vec3f;
class Matrix44f; // Forward definition to allow use in Vec3f

class Vec3f {
private:
public:
	float x, y, z, origX, origY, origZ; // Maybe these should be private, but we have global variables, why not public data members

	Vec3f() { x = 0.0f; y = 0.0f; z = 0.0f; };
	Vec3f(float nX, float nY, float nZ) : x(nX), y(nY), z(nZ) {};

	void rotateXZ(float);
	void rotateY(float);

	Vec3f& operator*=(Matrix44f);
	float operator[](int);

	friend ostream& operator<<(ostream&, Vec3f&);
};

class Matrix44f {
private:
public:
	float matrix[4][4];

	Matrix44f();
	Matrix44f(float array[4][4]);

	friend ostream& operator<<(ostream&, Matrix44f&);
};

#endif