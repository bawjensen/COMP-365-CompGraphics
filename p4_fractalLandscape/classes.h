#ifndef _CLASSES
#define _CLASSES

#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES // Needed to get MATH_PI from cmath
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Matrix44f; // Forward definition to allow use in Vec3f

class Vec3f {
private:
public:
	float x, y, z, origX, origY, origZ; // Maybe these should be private, but we have global variables, why not public data members
	float xzAngle;

	Vec3f();
	Vec3f(float nX, float nY, float nZ);

	void set(float, float, float);

	Vec3f rotateZ(float);
	Vec3f rotateY(float);

	Vec3f& operator*=(Matrix44f);
	Vec3f& operator*=(float);
	Vec3f& operator=(const Vec3f&);
	Vec3f operator-(const Vec3f&);
	Vec3f operator+(const Vec3f&);
	Vec3f& operator+=(const Vec3f&);
	Vec3f& operator-=(const Vec3f&);
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

class Camera {
private:
public:
	static const int FORWARD = 0;
	static const int BACKWARD = 1;
	static const int LEFT = 2;
	static const int RIGHT = 3;
	static const int UP = 4;
	static const int DOWN = 5;

	Vec3f pos, origPos;
	Vec3f focus;
	Vec3f viewDir, origViewDir;
	Vec3f pOrigin;
	bool isFocusing, panActive;
	float horizAngle, vertAngle;
	float deltaHorizAngle, deltaVertAngle;
	float angularScrollSpeed;
	float rotationRadius;
	float depthOfView;

	Camera();

	void setPos(float, float, float);
	void setViewDir(float, float, float);
	void setFocus(float, float, float);
	void setRotationRadius(int);

	void move(int);

	void rotateTo(float, float);
	void rotate(float, float);
	void handleClick(int, int, int, int);
	void handleMovement(int, int);
};

class Spline {
private:
public:
	static const int TYPE_KNOT = 0;
	static const int TYPE_LINEAR = 1;
	static const int TYPE_QUADRATIC = 2;

	Vec3f* pArray;
	int type;
	int length;
	float prevSlope;

	Spline();
	~Spline();

	void create(int, float[], int, int, int, bool crossWays=false);
	float* interpolate(int, int, int, bool linear=false);
	void display(float heightFactor=1.0f);
};

class SplineGrid {
private:
public:
	static const int NUM_MODES = 3;

	static const int MODE_KNOTS = 0;
	static const int MODE_LINEAR = 1;
	static const int MODE_QUADRATIC = 2;

	int nCols, nRows;
	int xOffset, yOffset;
	int mode;
	int greaterDimension;
	float elevFactor;
	float xllCorner, yllCorner;
	float cellSize;
	float highest, lowest;
	float** dataArray;
	bool initialized[10];
	vector<Spline>* splineVectorArray;

	SplineGrid();
	~SplineGrid();

	void setMode(int);
	void changeGridElevation(float);

	// void initializeKnots(bool forLinear=false);
	// void initializeLinear();
	// void initializeQuadratic();

	void readFromESRIFile(string);
	void initialize(int);
	void display();
};

#endif