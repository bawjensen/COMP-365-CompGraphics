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
#include <map>
#include <random>
#include <vector>

using namespace std;

class Matrix44f; // Forward definition to allow use in Vec3f

class Vec3f {
private:
public:
	float x, y, z; // Maybe these should be private, but we have global variables, why not public data members
	float xzAngle;

	Vec3f();
	Vec3f(float nX, float nY, float nZ);

	void set(float, float, float);

	Vec3f rotateX(float);
	Vec3f rotateY(float);
	Vec3f rotateZ(float);

	Vec3f& operator*=(Matrix44f);
	Vec3f& operator*=(float);
	Vec3f& operator=(const Vec3f&);
	Vec3f operator-(const Vec3f&);
	Vec3f operator+(const Vec3f&);
	Vec3f operator*(const float) const;
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

class Ground {
private:
public:
	float** pointGrid;
	int nCols, nRows;
	int xOffset, yOffset;
	int greaterDimension;
	float highest, lowest;
	float firstDelimiter, secondDelimiter;
	float elevFactor;
	float xllCorner, yllCorner;
	float cellSize;

	Ground();

	void readFromESRIFile(string);
	void display();
	void setGreen();
	void setGray();
	void setWhite();

	float heightAt(float, float);
};

class PlantGrammar {
private:
public:
	int depthLimit;
	float angle, length;
	string start;
	map< char, vector<string> > grammarMap;

	PlantGrammar();

	void loadFromFile(string);
};

class Plant {
private:
public:
	Vec3f startPos;
	string plantString;
	float angle, length;

	Plant();
	Plant(int, int, int, string, float, float);

	void display();
	void rotateX(string);
	void rotateY(string);
	void drawBranch();
	void drawFlower();
	void drawLeaf();
};

class PlantLandscape {
private:
public:
	vector<Plant> plantVec;
	PlantGrammar grammarArray[10];
	int nGrammars;

	PlantLandscape();

	void display();
	void loadGrammar(string);
	string generatePlantString(int);
	void addPlant(int, int, int, int);
	void handleClick(int, int, int, int);
};

class Minimap {
private:
public:
	int width, height;
	int upperRightX, upperRightY;
	int left, right;
	int top, bottom;
	Ground* groundPointer;
	PlantLandscape* plantLandPointer;
	Vec3f indicator;

	Minimap();
	Minimap(int, int, int, int, Ground*, PlantLandscape*);

	void display();
	void displayIndicator();
	void handleClick(int, int, int, int);
	void handleMovement(int, int);
};

#endif