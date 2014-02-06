#include "constants.cpp"

using namespace std;

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

class AxisAlignedBB {
private:
	int minX, minY, maxX, maxY;
public:
	AxisAlignedBB() {};

	void operator()(int, int, int, int);
	bool surrounds(CoordPoint);
};

void AxisAlignedBB::operator()(int nX, int nY, int xX, int xY) {
	this->minX = nX;
	this->minY = nY;
	this->maxX = xX;
	this->maxY = xY;
}

bool AxisAlignedBB::surrounds(CoordPoint point) {
	// cout << "Finding " << point.getX() << ", " << point.getY() << endl;
	// cout << "In: " << this->minX << " - " << this->maxX << 
	// 		" by " << this->minY << " - " << this->maxY << endl;
	return (point.getX() >= this->minX && point.getX() <= this->maxX && point.getY() >= this->minY && point.getY() <= this->maxY );
}

class Building {
private:
	vector<CoordPoint> corners;
	AxisAlignedBB boundingBox;
	string label;
	int type;
public:
	Building() { this->label = "nada"; };

	void setType(int newType) { this->type = newType; };
	void setLabel(string newLabel) { this->label = newLabel; };

	int getType() { return this->type; };
	string getLabel() { return this->label; };

	void draw(bool highlighted=false);
	void reset() { this->corners.clear(); };
	void addCorner(int x, int y) { this->corners.push_back( CoordPoint(x, y) ); };
	void calculateAABB(); // Axis-Aligned Bounding Box
	string writeBuilding();
	bool contains(CoordPoint);
};

void Building::draw(bool highlighted) {
	if (highlighted) glColor3f(1.0, 1.0, 1.0);
	else if (this->type == BUILDING_GENERAL) glColor3f(0.0, 0.75, 0.25);
	else if (this->type == BUILDING_DORM) glColor3f(0.0, 0.25, 0.75);
	else if (this->type == BUILDING_FIELD) glColor3f(0.0, 0.5, 0.0);
	// else if (this->type == BUILDING_ROAD) glColor3f(0.5, 0.25, 0.1);
	else if (this->type == BUILDING_ROAD) glColor3f(0.0, 0.0, 0.2);
	else if (this->type == BUILDING_WATER) glColor3f(0.0, 0.0, 0.25);
	else if (this->type == BUILDING_PARKINGLOT) glColor3f(0.0, 0.25, 0.25);

	glBegin(GL_POLYGON);
	for (vector<CoordPoint>::iterator it = this->corners.begin(); it < this->corners.end(); it++) {
		glVertex2i( it->getX(), it->getY() );
	}
	glEnd();

	glColor3f(0.0, 0.0, 0.2);
	// glLineWidth((GLfloat)2);
	glBegin(GL_LINE_LOOP);
	for (vector<CoordPoint>::iterator it = this->corners.begin(); it < this->corners.end(); it++) {
		glVertex2i( it->getX(), it->getY() );
	}
	glEnd();
}

void Building::calculateAABB() {
	int newX, newY, maxX, maxY, minX, minY;

	vector<CoordPoint>::iterator it = this->corners.begin();

	minX = it->getX();
	maxX = it->getX();
	minY = it->getY();
	maxY = it->getY();
	it++;

	for (; it < this->corners.end(); it++) {
		newX = it->getX();
		newY = it->getY();

		maxX = newX > maxX ? newX : maxX; // Replace max with new if it's larger
		minX = newX < minX ? newX : minX; // Replace min with new if it's smaller
		maxY = newY > maxY ? newY : maxY; // Replace max with new if it's larger
		minY = newY < minY ? newY : minY; // Replace min with new if it's smaller
	}

	this->boundingBox(minX, minY, maxX, maxY);
}

bool Building::contains(CoordPoint point) {
	if (this->type == BUILDING_ROAD)
		return false;
	else
		return this->boundingBox.surrounds(point);
}

string Building::writeBuilding() {
	stringstream outString;

	outString << this->label << " " << this->type << " ";

	for (vector<CoordPoint>::iterator it = this->corners.begin(); it < this->corners.end(); it++) {
		outString << it->getX() << "," << it->getY() << " ";
	}
	return outString.str();
}