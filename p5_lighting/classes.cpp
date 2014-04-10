#include "classes.h"

// -------------------------------------------------------------------------------------------

Camera::Camera() {
	this->pos = Vec3f(-100.0f, 25.0f, 0.0f); // Camera position
	this->origViewDir = Vec3f(1.0f, 0.0f, 0.0f); // View direction
	this->viewDir = this->origViewDir; // View direction
	this->strafeVec = this->viewDir.rotateY(-M_PI / 2);

	// Hard coding values for original view
	// this->vertAngle = -M_PI / 6;
	// this->viewDir = this->viewDir.rotateX(-M_PI / 6);

	this->isFocusing = false; // Whether or not the camera is "viewing" or "focusing"
	this->focus = Vec3f(0.0f, 0.0f, 0.0f); // Where the camera is focusing

	this->angularScrollSpeed = 1 / (float)200; // Rotation speed of the camera
	this->panActive = false; // Whether or not the camera is panning (via the mouse)
}

void Camera::setPos(float pX, float pY, float pZ) {
	this->origPos = Vec3f(pX, pY, pZ);
	this->pos = this->origPos;
	this->rotationRadius = max(pX, pZ);
	this->depthOfView = 2 * this->rotationRadius;
}

void Camera::setViewDir(float vX, float vY, float vZ) {
	this->viewDir = Vec3f(vX, vY, vZ);
}

void Camera::setFocus(float fX, float fY, float fZ) {
	this->focus = Vec3f(fX, fY, fZ);
}

void Camera::setRotationRadius(int r) {
	float radiusRatio = (float)r / this->rotationRadius;

	Vec3f distFromFocus = this->origPos - this->focus;

	distFromFocus *= radiusRatio;
	this->origPos = distFromFocus + this->focus;
	this->rotate(0, 0); // Call a dummy rotate to update the camera's position

	this->rotationRadius = r;
	this->depthOfView = 2 * this->rotationRadius;
}

void Camera::setDepthOfView(float dist) {
	this->depthOfView = dist;
}

void Camera::update() {
	float moveSpeedFactor = 0.5;
	if (this->moving[Camera::FORWARD]) {
		this->pos += this->viewDir * moveSpeedFactor;
	}
	if (this->moving[Camera::BACKWARD]) {
		this->pos -= this->viewDir * moveSpeedFactor;
	}
	if (this->moving[Camera::LEFT]) {
		this->pos += this->strafeVec * moveSpeedFactor;
	}
	if (this->moving[Camera::RIGHT]) {
		this->pos -= this->strafeVec * moveSpeedFactor;
	}
	if (this->moving[Camera::UP]) {
		this->pos += Camera::UP_VECT * moveSpeedFactor;
	}
	if (this->moving[Camera::DOWN]) {
		this->pos -= Camera::UP_VECT * moveSpeedFactor;
	}
}

void Camera::stopMove(int direction) {
	this->moving[direction] = false;
}

void Camera::move(int direction) {
	this->moving[direction] = true;
}

void Camera::rotateTo(float hAngle, float vAngle) {
	this->horizAngle = hAngle;
	this->vertAngle = vAngle;

	this->pos = this->origPos.rotateY(this->horizAngle);
}

void Camera::rotate(float hAngle, float vAngle) {
	this->horizAngle += hAngle;
	this->vertAngle += vAngle;

	float angleLimit = (M_PI / 2) - .01;

	if (this->vertAngle > angleLimit) this->vertAngle = angleLimit;
	else if (this->vertAngle < -angleLimit) this->vertAngle = -angleLimit;

	this->viewDir = this->origViewDir.rotateZ(-this->vertAngle).rotateY(this->horizAngle);
	this->strafeVec = this->strafeVec.rotateY(hAngle);
}

void Camera::handleClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) { // only start motion if the left button is pressed
		if (state == GLUT_UP) { // when the button is released
			this->panActive = false;
		}
		else  { // state = GLUT_DOWN
			this->pOrigin = Vec3f(x, y, 0); // Ignore z for simplicity
			this->panActive = true;
		}
	}
}

void Camera::handleMovement(int x, int y) {
	if (panActive) { // this will only be true when the left button is down

		// update deltaHorizAngle
		this->deltaHorizAngle = -(this->pOrigin.x - x) * this->angularScrollSpeed;
		this->pOrigin.x = x;

		// update deltaVertAngle
		this->deltaVertAngle = (this->pOrigin.y - y) * angularScrollSpeed;
		this->pOrigin.y = y;

		this->rotate(deltaHorizAngle, deltaVertAngle);
	}
}

// -------------------------------------------------------------------------------------------

Triangle::Triangle() {

}

Triangle::Triangle(Coord3f nP1, Coord3f nP2, Coord3f nP3, Color3f nC1, Color3f nC2, Color3f nC3, Coord3f nN1, Coord3f nN2, Coord3f nN3) {
	this->p1 = nP1;
	this->p2 = nP2;
	this->p3 = nP3;

	this->c1 = nC1;
	this->c2 = nC2;
	this->c3 = nC3;

	this->n1 = nN1;
	this->n2 = nN2;
	this->n3 = nN3;
}

void Triangle::display() {
	glEnd();
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p1.x + n1.x, p1.y + n1.y, p1.z + n1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p2.x + n2.x, p2.y + n2.y, p2.z + n2.z);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p3.x + n3.x, p3.y + n3.y, p3.z + n3.z);
	glEnd();
	glEnable(GL_LIGHTING);
	glBegin(GL_TRIANGLES);

	glNormal3f(n1.x, n1.y, n1.z);
	glColor3f(c1.red, c1.green, c1.blue);
	glVertex3f(p1.x, p1.y, p1.z);

	glNormal3f(n2.x, n2.y, n2.z);
	glColor3f(c2.red, c2.green, c2.blue);
	glVertex3f(p2.x, p2.y, p2.z);

	glNormal3f(n3.x, n3.y, n3.z);
	glColor3f(c3.red, c3.green, c3.blue);
	glVertex3f(p3.x, p3.y, p3.z);
}

// -------------------------------------------------------------------------------------------

Ground::Ground() {

}

void Ground::readFromESRIFile(string filename) {
	if (this->pointGrid) {
		for (int row = 0; row < nRows; row++) {
			if (this->pointGrid[row])
				delete[] this->pointGrid[row];
		}
		delete[] this->pointGrid;
	}

	ifstream inFile;

	inFile.open(filename.c_str());

	if (!inFile) {
		cout << "File " << filename << " didn't exist" << endl;
		exit(1);
	}

	string labelTrash;
	int trash;
	float labelValue;

	inFile >> labelTrash >> labelValue;
	this->nCols = (int)labelValue;

	inFile >> labelTrash >> labelValue;
	this->nRows = (int)labelValue;

	inFile >> labelTrash >> labelValue;
	trash = labelValue;

	inFile >> labelTrash >> labelValue;
	trash = labelValue;

	inFile >> labelTrash >> labelValue;
	this->cellSize = labelValue;

	this->pointGrid = new float*[nRows];
	for (int row = 0; row < nRows; row++) {
		this->pointGrid[row] = new float[nCols];
	}

	float cellData;
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			inFile >> cellData;
			if (cellData > highest) highest = cellData;
			if (cellData < lowest) lowest = cellData;
			this->pointGrid[i][j] = cellData;
		}
	}

	this->iOffset = -(this->nRows - 1) / (float)2;
	this->jOffset = -(this->nCols - 1) / (float)2;

	this->greaterDimension = max(this->nCols, this->nRows) * this->cellSize;

	this->firstDelimiter = (this->highest - this->lowest) / 4;
	this->secondDelimiter = 3 * (this->highest - this->lowest) / 4;

	this->triangulateForDisplay();
}

void Ground::triangulateForDisplay() {
	// i, j = 0, 0 is the lower left corner of the array
	for (int i = 0; i < (this->nRows - 1); i++) {
		for (int j = 0; j < (this->nCols - 1); j++) {

			Coord2i ll(i, j);		// Lower left
			Coord2i lr(i, j+1);		// Lower right
			Coord2i ul(i+1, j);		// Upper left
			Coord2i ur(i+1, j+1);	// Upper right

			Coord3f p1 = this->toCoord(ll);
			Coord3f p2 = this->toCoord(lr);
			Coord3f p3 = this->toCoord(ur);
			Coord3f p4 = this->toCoord(ul);

			Color3f c1 = this->colorAt(p1);
			Color3f c2 = this->colorAt(p2);
			Color3f c3 = this->colorAt(p3);
			Color3f c4 = this->colorAt(p4);

			cout << "Point ll: (" << ll.x << ", " << this->pointGrid[ll.x][ll.y] << ", " << ll.y << ")" << endl;
			cout << "Point lr: (" << lr.x << ", " << this->pointGrid[lr.x][lr.y] << ", " << lr.y << ")" << endl;
			cout << "Point ul: (" << ul.x << ", " << this->pointGrid[ul.x][ul.y] << ", " << ul.y << ")" << endl;
			cout << "Point ur: (" << ur.x << ", " << this->pointGrid[ur.x][ur.y] << ", " << ur.y << ")" << endl;

			Coord3f n1 = this->normalAt(ll);
			Coord3f n2 = this->normalAt(lr);
			Coord3f n3 = this->normalAt(ur);
			Coord3f n4 = this->normalAt(ul);

			if (j == 2)
				exit(1);


			this->displayVector.push_back(Triangle(p1, p3, p4, c1, c3, c4, n1, n3, n4));
			this->displayVector.push_back(Triangle(p1, p2, p3, c1, c2, c3, n1, n2, n3));
		}
	}

	// exit(1);
}

Color3f Ground::colorAt(Coord3f point) {
	float& height = point.y;

	if (height >= this->lowest and height < this->firstDelimiter) {
		return Color3f(0.1, 0.3, 0.1);
	}
	else if (height >= this->firstDelimiter and height < this->secondDelimiter) {
		return Color3f(0.3, 0.3, 0.3);
	}
	else {
		return Color3f(1.0, 1.0, 1.0);
	}
}

Coord3f Ground::normalAt(Coord2i indexPoint) {
	// NOTE: In this function, the vertical component is i, which comes in as indexPoint.x,
	// 		 and vice versa for the horizonal, j, which is indexPoint.y

	// cout << "Doing normal for: " << indexPoint.y << "," << indexPoint.x << endl;
	Coord2i p1, p2, p3;
	Coord3f normal;
	bool ignore[6] = { false, false, false, false, false, false };

	float& cS = this->cellSize;

	if (indexPoint.x == 0) {
		ignore[2] = true;
		ignore[3] = true;
		ignore[4] = true;
	}
	if (indexPoint.x == (this->nRows - 1)) {
		ignore[0] = true;
		ignore[1] = true;
		ignore[5] = true;
	}
	if (indexPoint.y == 0) {
		ignore[3] = true;
		ignore[4] = true;
		ignore[5] = true;
	}
	if (indexPoint.y == (this->nCols - 1)) {
		ignore[0] = true;
		ignore[1] = true;
		ignore[2] = true;
	}

	vector<Coord3f> normals;

	p1 = Coord2i(indexPoint.x, indexPoint.y);

	for (int i = 0; i < 6; i++) {
		if (ignore[i]) continue;

		switch(i) {
			case 0:	p2 = Coord2i(indexPoint.x+1, indexPoint.y+1);
					p3 = Coord2i(indexPoint.x+1, indexPoint.y);
					break;

			case 1:	p2 = Coord2i(indexPoint.x, indexPoint.y+1);
					p3 = Coord2i(indexPoint.x+1, indexPoint.y+1);
					break;

			case 2:	p2 = Coord2i(indexPoint.x-1, indexPoint.y);
					p3 = Coord2i(indexPoint.x, indexPoint.y+1);
					break;

			case 3:	p2 = Coord2i(indexPoint.x-1, indexPoint.y-1);
					p3 = Coord2i(indexPoint.x-1, indexPoint.y);
					break;

			case 4:	p2 = Coord2i(indexPoint.x, indexPoint.y-1);
					p3 = Coord2i(indexPoint.x-1, indexPoint.y-1);
					break;

			case 5:	p2 = Coord2i(indexPoint.x+1, indexPoint.y);
					p3 = Coord2i(indexPoint.x, indexPoint.y-1);
					break;
		}

		normal = Coord3f(cS * (this->pointGrid[p3.x][p3.y] - this->pointGrid[p2.x][p2.y]),
			cS * cS,
			cS * (this->pointGrid[p1.x][p1.y] - this->pointGrid[p3.x][p3.y]) );

		cout << "P1: (" << p1.x << ", " << this->pointGrid[p1.x][p1.y] << ", " << p1.y << ")" << endl;
		cout << "P2: (" << p2.x << ", " << this->pointGrid[p2.x][p2.y] << ", " << p2.y << ")" << endl;
		cout << "P3: (" << p3.x << ", " << this->pointGrid[p3.x][p3.y] << ", " << p3.y << ")" << endl;
		cout << "Normal: " << normal << endl;

		normals.push_back(normal);
	}

	Coord3f sumNormal;
	for (vector<Coord3f>::iterator it = normals.begin(); it != normals.end(); ++it) {
		sumNormal = sumNormal + (*it);
	}

	cout << "Sum: " << this->normalize(sumNormal) << endl;
	return this->normalize(sumNormal);
}

Coord3f Ground::toCoord(Coord2i indexPoint) {
	float x = (indexPoint.x + this->iOffset) * this->cellSize;
	float y = this->pointGrid[indexPoint.x][indexPoint.y];
	float z = (indexPoint.y + this->jOffset) * this->cellSize;

	return Coord3f(x, y, z);
}

Coord2i Ground::toIndex(Vec3f point) {

}

Coord3f Ground::normalize(Coord3f point) {
	return point / sqrt(point.x * point.x + point.y * point.y + point.z * point.z);
}

void Ground::setGreen() {
	glColor3f(0.1, 0.3, 0.1);
}

void Ground::setGray() {
	glColor3f(0.3, 0.3, 0.3);
}

void Ground::setWhite() {
	glColor3f(1.0, 1.0, 1.0);
}

void Ground::display() {
	glBegin(GL_TRIANGLES);
	for (vector<Triangle>::iterator it = this->displayVector.begin(); it != this->displayVector.end(); ++it) {
		it->display();
	}
	glEnd();
}

float Ground::heightAt(float x, float y) {
	// Note: i is y, not x, and vice versa with j
	float result;

	float iPrime = y;
	float jPrime = x;

	int i = (int)iPrime;
	int j = (int)jPrime;

	if (i+1 >= this->nRows or j+1 >= this->nCols) return 0;

	float v1 = this->pointGrid[i][j];
	float v2 = this->pointGrid[i][j+1];
	float v3 = this->pointGrid[i+1][j+1];
	float v4 = this->pointGrid[i+1][j];

	if ((iPrime - (int)iPrime) < (jPrime - (int)jPrime)) {
		result = v1 + (v3 - v2)*(iPrime - i) + (v2 - v1)*(jPrime - j);
	}
	else {
		result = v1 + (v4 - v1)*(iPrime - i) + (v3 - v4)*(jPrime - j);
	}

	return result;
}