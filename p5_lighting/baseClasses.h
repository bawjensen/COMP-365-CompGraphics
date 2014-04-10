#ifndef _BASECLASSES
#define _BASECLASSES

#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES // Needed to get MATH_PI from cmath
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

class Matrix44f; // Forward definition to allow use in Vec3f

class Coord3f {
private:
public:
	float x, y, z;

	Coord3f();
};

class Vec3f : public Coord3f {
private:
public:
	float xzAngle;

	Vec3f();
	Vec3f(float nX, float nY, float nZ);

	void set(float, float, float);

	Vec3f rotateX(float);
	Vec3f rotateY(float);
	Vec3f rotateZ(float);

	Vec3f cross(const Vec3f&) const;

	Vec3f& operator*=(Matrix44f);
	Vec3f& operator*=(float);
	Vec3f& operator+=(const Vec3f&);
	Vec3f& operator-=(const Vec3f&);
	Vec3f& operator=(const Vec3f&);
	Vec3f operator-(const Vec3f&) const;
	Vec3f operator+(const Vec3f&) const;
	Vec3f operator*(const float) const;
	float operator[](int);

	friend ostream& operator<<(ostream&, Vec3f&);
};

class Matrix44f {
private:
public:
	float matrix[4][4];

	Matrix44f();
	Matrix44f(float array[4][4]);

	Matrix44f& operator*=(Matrix44f);
	Matrix44f& operator=(const Matrix44f&);

	friend ostream& operator<<(ostream&, Matrix44f&);
};

#endif