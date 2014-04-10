#ifndef _CLASSES
#define _CLASSES

#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <random>

#include "baseClasses.h"

using namespace std;

class Camera {
private:
public:
	static const int FORWARD = 0;
	static const int BACKWARD = 1;
	static const int LEFT = 2;
	static const int RIGHT = 3;
	static const int UP = 4;
	static const int DOWN = 5;

	const Vec3f UP_VECT = Vec3f(0.0, 1.0, 0.0);

	Vec3f pos, origPos; 		// Position and original position vectors
	Vec3f focus;				// Where the camera is focus-looking (aka the focus doesn't move, the camera rotates around)
	Vec3f viewDir, origViewDir; // Where the camera is looking, and the original value
	Vec3f strafeVec;			//
	Vec3f pOrigin;
	bool isFocusing, panActive; // Booleans for whether or not the camera is a certain mode
	bool moving[10]; 			// Boolean array for status of movement in up to 10 directions
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
	void setDepthOfView(float);

	void update();
	void move(int);
	void stopMove(int);

	void rotateTo(float, float);
	void rotate(float, float);
	void handleClick(int, int, int, int);
	void handleMovement(int, int);
};

class User : public Camera {
private:
public:

};

class Triangle {
private:
public:
	Coord3f p1, p2, p3; // 3 points of the triangle
	Coord3f n1, n2, n3; // 3 normals corresponding to the points of the triangle

	Color3f c1, c2, c3; // 3 colors corresponding to the points of the triangle

	Triangle();
	Triangle(Coord3f, Coord3f, Coord3f, Color3f, Color3f, Color3f, Coord3f, Coord3f, Coord3f);

	void display();
};

class Ground {
private:
public:
	float** pointGrid;
	int nCols, nRows;
	float iOffset, jOffset;
	int greaterDimension;
	float highest, lowest;
	float firstDelimiter, secondDelimiter;
	float cellSize;

	vector<Triangle> displayVector;

	Ground();

	void readFromESRIFile(string);
	void triangulateForDisplay();
	void display();
	Coord2i toIndex(Vec3f);
	Color3f colorAt(Coord3f);
	Coord3f normalAt(Coord2i);
	Coord3f toCoord(Coord2i);
	Coord3f normalize(Coord3f);
	void setGreen();
	void setGray();
	void setWhite();

	float heightAt(float, float);
};

class DEMGenerator {
private:
public:
	string outFileName;
	float roughnessFactor;
	default_random_engine generator;
	normal_distribution<float> distribution;
	int gridWidth;
	float cellSize;
	float stdDev;

	DEMGenerator();

	float randVal(int);
	void fractalRecurse(float**, int, int, int, int);
	float** generateGrid(int);
	string createGridFile();
};

#endif