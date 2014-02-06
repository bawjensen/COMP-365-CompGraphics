#ifndef _CLASSES
#define _CLASSES

#include <GL/glut.h>

#include "constants.h"

class CoordPoint {
private:
	int x, y;
public:
	CoordPoint() { x = 0; y = 0; };
	CoordPoint(int newX, int newY) { x = newX; y = newY; };

	void operator()(int newX, int newY) { x = newX; y = newY; };

	int getX() { return this->x; };
	int getY() { return this->y; };
};

class AxisAlignedBB { // Axis-Aligned Bounding Box
private:
	int minX, minY, maxX, maxY;
public:
	AxisAlignedBB() {};

	void operator()(int, int, int, int);
	bool surrounds(CoordPoint);
	CoordPoint getMiddle();
};

class Building {
private:
	vector<CoordPoint> corners;
	AxisAlignedBB boundingBox;
	string label;
	int type;
public:
	Building() { this->label = "blank_label"; };

	void setType(int newType) { this->type = newType; };
	void setLabel(string newLabel) { this->label = newLabel; };

	int getType() { return this->type; };
	string getLabel() { return this->label; };

	void draw(bool highlighted=false, bool labelsOn=false, float currentScale=1.0);
	void drawShape(bool);
	void drawOutline();
	void drawLabel(bool, float);
	void reset() { this->corners.clear(); };
	void addCorner(int x, int y) { this->corners.push_back( CoordPoint(x, y) ); };
	void calculateAABB(); // (Axis-Aligned Bounding Box)
	string writeBuilding();
	bool contains(CoordPoint);
};

#endif