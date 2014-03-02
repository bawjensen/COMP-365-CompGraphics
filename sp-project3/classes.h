#ifndef _CLASSES
#define _CLASSES

class Vec3f {
private:
public:
	float x, y, z; // Maybe these should be private, but we have global variables, why not public data members

	Vec3f() { x = 0.0f; y = 0.0f; z = 0.0f; };
	Vec3f(float nX, float nY, float nZ) : x(nX), y(nY), z(nZ) {};

	void rotateX(float);

	Vec3f operator*(Matrix44f);
};

class Matrix44f {
private:
public:
	float matrix[4][4];

};

#endif