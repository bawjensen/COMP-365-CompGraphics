#include "classes.h"

using namespace std;

void AxisAlignedBB::operator()(int nX, int nY, int xX, int xY) {
	this->minX = nX;
	this->minY = nY;
	this->maxX = xX;
	this->maxY = xY;
}

bool AxisAlignedBB::surrounds(CoordPoint point) {
	return (point.getX() >= this->minX && point.getX() <= this->maxX && point.getY() >= this->minY && point.getY() <= this->maxY );
}

CoordPoint AxisAlignedBB::getMiddle() {
	int midX = (int)(this->minX + this->maxX) / 2;
	int midY = (int)(this->minY + this->maxY) / 2;
	return CoordPoint(midX, midY);
}

void Building::draw(bool highlighted, bool labelsOn, float currentScale) {
	this->drawShape(highlighted);
	this->drawOutline();
	this->drawLabel(labelsOn, currentScale);
}

void Building::drawShape(bool highlighted) {
	if (highlighted) glColor3f(1.0, 1.0, 0.0);
	else if (this->type == BUILDING_GENERAL) glColor3f(0.0, 0.75, 0.25);
	else if (this->type == BUILDING_DORM) glColor3f(0.0, 0.25, 0.75);
	else if (this->type == BUILDING_FIELD) glColor3f(0.0, 0.5, 0.0);
	// else if (this->type == BUILDING_ROAD) glColor3f(0.5, 0.25, 0.1);
	else if (this->type == BUILDING_ROAD) glColor3f(0.0, 0.0, 0.2);
	else if (this->type == BUILDING_WATER) glColor3f(0.0, 0.0, 0.25);
	else if (this->type == BUILDING_PARKINGLOT) glColor3f(0.0, 0.25, 0.25);

	// Draw colored shape for building
	glBegin(GL_POLYGON);
	for (vector<CoordPoint>::iterator it = this->corners.begin(); it < this->corners.end(); it++) {
		glVertex2i( it->getX(), it->getY() );
	}
	glEnd();
}

void Building::drawOutline() {
	// Draw black outline (for looks)
	glColor3f(0.0, 0.0, 0.2);
	glBegin(GL_LINE_LOOP);
	for (vector<CoordPoint>::iterator it = this->corners.begin(); it < this->corners.end(); it++) {
		glVertex2i( it->getX(), it->getY() );
	}
	glEnd();
}

void Building::drawLabel(bool labelsOn, float currentScale) {
	if (!labelsOn || this->type == BUILDING_ROAD || this->type == BUILDING_PARKINGLOT ) return;

	int charWidth = 9;
	int charHeight = 15;

	int textWidth = charWidth * this->label.length() / currentScale;
	int textHeight = charHeight / currentScale;

	int labelPadding = 2;

	CoordPoint midPoint = this->boundingBox.getMiddle();

	int xPos = midPoint.getX() - (textWidth / 2);
	int yPos = midPoint.getY() - (textHeight / 2);

	// Display background
	glColor4f(0.0, 0.0, 0.0, 0.75);
	glBegin(GL_POLYGON);
		glVertex2i(xPos - labelPadding, yPos - labelPadding);
		glVertex2i(xPos + textWidth + labelPadding, yPos - labelPadding);
		glVertex2i(xPos + textWidth + labelPadding, yPos + textHeight + labelPadding);
		glVertex2i(xPos - labelPadding, yPos + textHeight + labelPadding);
	glEnd();

	// Display text in front of background
	glColor3f(1.0, 1.0, 1.0);

	glRasterPos2i (xPos, yPos);
	for (string::iterator it = this->label.begin(); it != this->label.end(); it++) {
		glutBitmapCharacter (GLUT_BITMAP_9_BY_15, *it);
	}
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