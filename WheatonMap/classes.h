#ifndef _CLASSES
#define _CLASSES

#include <GL/glut.h>

#include "constants.h"

/*
A simple class created to store (x, y) value pairs denoting coordinate points
in a cartesian coordinate system.
*/
class CoordPoint {
private:
	int x, y; // The x, y position of the point (in pixels)
public:
	CoordPoint() { x = 0; y = 0; }; // Default CTOR
	CoordPoint(int newX, int newY) { x = newX; y = newY; }; // Parameterized CTOR

	void operator()(int newX, int newY) { x = newX; y = newY; }; // Setting for both x and y

	int getX() { return this->x; }; // Getter for x
	int getY() { return this->y; }; // Getter for y
};

/*
Simple class to define a bounding box (usually around a building), used in rudementary collision
detection. 
*/
class AxisAlignedBB { // Axis-Aligned Bounding Box
private:
	int minX, minY, maxX, maxY; // Four lines to define a box
public:
	AxisAlignedBB() {}; // Default CTOR

	void operator()(int, int, int, int); // Setter for all four attributes
	bool surrounds(CoordPoint); // Detection to see if point is in bounding box
	CoordPoint getMiddle(); // Get the mid-point of the bounding box
};

/*
Class to define any multi-sided shape on the map, even roads. Used for storing and displaying
the map.
*/
class Building {
private:
	vector<CoordPoint> corners; // A vector of all the corner points of the building
	AxisAlignedBB boundingBox; // Bounding box that completely surrounds the building
	string label; // Label for the building
	int type; // Type of building (types defined in "constants.h")
public:
	Building() { this->label = "blank_label"; }; // Default CTOR

	void setType(int newType) { this->type = newType; }; // Setter for type attribute
	void setLabel(string newLabel) { this->label = newLabel; }; // Setter for label attribute

	int getType() { return this->type; }; // Getter for type
	string getLabel() { return this->label; }; // Getter for label

	void draw(bool highlighted=false, bool labelsOn=false, float currentScale=1.0); // Method to draw the building
																					// to the GLUT window
	void drawShape(bool); // Draws the actual shape of the building using a solid filled polygon
	void drawOutline(); // Draws an outline around the building
	void drawLabel(bool, float); // Draws the label inside the building
	void reset() { this->corners.clear(); }; // Resets the building, clearing all corners from vector
	void addCorner(int x, int y) { this->corners.push_back( CoordPoint(x, y) ); }; // Adds a corner to a building
	void calculateAABB(); // (Axis-Aligned Bounding Box)
	string writeBuilding(); // Writes the content of the building to a formatted string for easy writing to file
	bool contains(CoordPoint); // Test to see if point is within the AABB of the building
};

#endif