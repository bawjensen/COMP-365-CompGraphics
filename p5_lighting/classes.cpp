#include "classes.h"

// -------------------------------------------------------------------------------------------

Camera::Camera() {
	this->pos = Vec3f(-100.0f, 25.0f, 0.0f); // Camera position
	this->origViewDir = Vec3f(1.0f, 0.0f, 0.0f); // View direction
	this->viewDir = this->origViewDir; // View direction
	this->strafeVec = this->viewDir.rotateY(-M_PI / 2);
	this->upVec = this->viewDir.rotateZ(-M_PI / 2);

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
	this->upVec = this->upVec.rotateZ(vAngle);
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
	// glEnd();
	// glDisable(GL_LIGHTING);
	// glColor3f(1.0, 0.0, 1.0);
	// glBegin(GL_LINES);
	// 	glVertex3f(p1.x, p1.y, p1.z);
	// 	glVertex3f(p1.x + n1.x, p1.y + n1.y, p1.z + n1.z);
	// 	glVertex3f(p2.x, p2.y, p2.z);
	// 	glVertex3f(p2.x + n2.x, p2.y + n2.y, p2.z + n2.z);
	// 	glVertex3f(p3.x, p3.y, p3.z);
	// 	glVertex3f(p3.x + n3.x, p3.y + n3.y, p3.z + n3.z);
	// glEnd();
	// glEnable(GL_LIGHTING);
	// glBegin(GL_TRIANGLES);


	glNormal3f(n1.x, n1.y, n1.z);
	float matDif1[] = { c1.red, c1.green, c1.blue, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif1);
	glVertex3f(p1.x, p1.y, p1.z);

	glNormal3f(n2.x, n2.y, n2.z);
	float matDif2[] = { c2.red, c2.green, c2.blue, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif2);
	glVertex3f(p2.x, p2.y, p2.z);

	glNormal3f(n3.x, n3.y, n3.z);
	float matDif3[] = { c3.red, c3.green, c3.blue, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif3);
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

	this->firstDelimiter = ((this->highest - this->lowest) / 2) + this->lowest;
	this->secondDelimiter = (3 * (this->highest - this->lowest) / 4) + this->lowest;

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

			Color3f c1 = this->colorAt(p1, ll);
			Color3f c2 = this->colorAt(p2, lr);
			Color3f c3 = this->colorAt(p3, ur);
			Color3f c4 = this->colorAt(p4, ul);

			Coord3f n1 = this->normalAt(ll);
			Coord3f n2 = this->normalAt(lr);
			Coord3f n3 = this->normalAt(ur);
			Coord3f n4 = this->normalAt(ul);

			this->displayVector.push_back(Triangle(p1, p3, p4,
												   c1, c3, c4,
												   n1, n3, n4));

			this->displayVector.push_back(Triangle(p1, p2, p3,
												   c1, c2, c3,
												   n1, n2, n3));
		}
	}

	// exit(1);
}

Color3f Ground::colorAt(Coord3f point, Coord2i indexPoint) {
	// NOTE: In this function, the vertical component is i, which comes in as indexPoint.x,
	// 		 and vice versa for the horizonal, j, which is indexPoint.y

	bool ignore[6] = { false, false, false, false, false, false };

	if (indexPoint.x == 0) {
		ignore[2] = true;
		ignore[3] = true;
	}
	if (indexPoint.x == (this->nRows - 1)) {
		ignore[0] = true;
		ignore[5] = true;
	}
	if (indexPoint.y == 0) {
		ignore[1] = true;
		ignore[2] = true;
	}
	if (indexPoint.y == (this->nCols - 1)) {
		ignore[2] = true;
		ignore[5] = true;
	}

	int dI, dJ;
	float dX;
	float maxSlope = 0; // The most slope
	float slope;
	bool isLake = true;

	for (int i = 0; i < 6; i++) {
		if (ignore[i]) continue;

		switch (i) {
			case 0: dI = 1;
					dJ = 0;
					dX = 1;
					break;
			case 1: dI = 0;
					dJ = -1;
					dX = 1;
					break;
			case 2: dI = -1;
					dJ = -1;
					dX = 1.414; // sqrt(2)
					break;
			case 3: dI = -1;
					dJ = 0;
					dX = 1;
					break;
			case 4: dI = 0;
					dJ = 1;
					dX = 1;
					break;
			case 5: dI = 1;
					dJ = 1;
					dX = 1.414; // sqrt(2)
					break;
		}

		slope = abs(this->pointGrid[indexPoint.x][indexPoint.y] - this->pointGrid[indexPoint.x + dI][indexPoint.y + dJ]) / dX;
		if (slope != 0.0f) isLake = false;
		if (slope > maxSlope) maxSlope = slope;
	}

	float degrees = atan(slope) * (180 / M_PI);

	if (degrees > 45) { // Angle > 60 means a grey cliff
		return Color3f(0.3, 0.3, 0.3);
	}
	else if (point.y > this->secondDelimiter) { // If above snowline, then white
		return Color3f(1.0, 1.0, 1.0);
	}
	else if (isLake) {
		return Color3f(0.3, 0.3, 1.0);
	}
	else {
		return Color3f(0.1, 0.3, 0.1);
	}
}

Coord3f Ground::normalAt(Coord2i indexPoint) {
	// NOTE: In this function, the vertical component is i, which comes in as indexPoint.x,
	// 		 and vice versa for the horizonal, j, which is indexPoint.y

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
			case 0:	p1 = Coord2i(indexPoint.x, indexPoint.y);
					p2 = Coord2i(indexPoint.x+1, indexPoint.y+1);
					p3 = Coord2i(indexPoint.x+1, indexPoint.y);
					break;

			case 1:	p1 = Coord2i(indexPoint.x, indexPoint.y);
					p2 = Coord2i(indexPoint.x, indexPoint.y+1);
					p3 = Coord2i(indexPoint.x+1, indexPoint.y+1);
					break;

			case 2:	p1 = Coord2i(indexPoint.x-1, indexPoint.y);
					p2 = Coord2i(indexPoint.x, indexPoint.y+1);
					p3 = Coord2i(indexPoint.x, indexPoint.y);
					break;

			case 3:	p1 = Coord2i(indexPoint.x-1, indexPoint.y-1);
					p2 = Coord2i(indexPoint.x-1, indexPoint.y);
					p3 = Coord2i(indexPoint.x, indexPoint.y);
					break;

			case 4:	p1 = Coord2i(indexPoint.x-1, indexPoint.y-1);
					p2 = Coord2i(indexPoint.x, indexPoint.y);
					p3 = Coord2i(indexPoint.x, indexPoint.y-1);
					break;

			case 5:	p1 = Coord2i(indexPoint.x, indexPoint.y-1);
					p2 = Coord2i(indexPoint.x, indexPoint.y);
					p3 = Coord2i(indexPoint.x+1, indexPoint.y);
					break;
		}

		if (i % 2 == 0) { // If i is even, then triangle is the upper half (if part of square) version
			normal = Coord3f(cS * (this->pointGrid[p1.x][p1.y] - this->pointGrid[p3.x][p3.y]),
							cS * cS,
							cS * (this->pointGrid[p3.x][p3.y] - this->pointGrid[p2.x][p2.y]));
		}
		else { // Otherwise, i is odd and the lower half
			normal = Coord3f(cS * (this->pointGrid[p2.x][p2.y] - this->pointGrid[p3.x][p3.y]),
							cS * cS,
							cS * (this->pointGrid[p1.x][p1.y] - this->pointGrid[p2.x][p2.y]));
		}

		normals.push_back(normal);
	}

	Coord3f sumNormal;
	for (vector<Coord3f>::iterator it = normals.begin(); it != normals.end(); ++it) {
		sumNormal = sumNormal + (*it);
	}

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

	// exit(1);
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

// -------------------------------------------------------------------------------------------

DEMGenerator::DEMGenerator() {
	this->outFileName = "my0.dem.grd";
	this->roughnessFactor = 2.5;
	this->gridWidth = 257;
	this->cellSize = 10.0;
	this->stdDev = 0.5;
	this->incrAmount = 2.0f;

	this->generator.seed(time(NULL));
}

void DEMGenerator::initialize() {
	this->grid = new float*[this->gridWidth];
	for (int i = 0; i < this->gridWidth; i++) {
		this->grid[i] = new float[this->gridWidth];
		for (int j = 0; j < this->gridWidth; j++) {
			this->grid[i][j] = -1.0f;
		}
	}
}

void DEMGenerator::smooth(float** grid) {
	float weight = 0.6;

	float tempGrid[this->gridWidth][this->gridWidth];

	for (int i = 1; i < this->gridWidth-1; i++) {
		for (int j = 1; j < this->gridWidth-1; j++) {
			float sum = weight * 
						(this->grid[i-1][j-1] + 
						 this->grid[i-1][j] + 
						 this->grid[i-1][j+1] + 
						 this->grid[i][j-1] + 
						 this->grid[i][j+1] + 
						 this->grid[i+1][j-1] + 
						 this->grid[i+1][j] + 
						 this->grid[i+1][j+1]) + this->grid[i][j];

			tempGrid[i][j] = sum / 9;
		}
	}

	for (int i = 1; i < this->gridWidth-1; i++) {
		for (int j = 1; j < this->gridWidth-1; j++) {
			this->grid[i][j] = tempGrid[i][j];
		}
	}
}

float DEMGenerator::randVal(int gridSize) {
	return this->roughnessFactor * gridSize * distribution(generator);
}

void DEMGenerator::fractalRecurse(int left, int right, int top, int bottom) {
	if ((right - left) < 2) {
		return;
	}

	int midPointH = (left + right) / 2;
	int midPointV = (top + bottom) / 2;

	if (this->grid[top][midPointH] == -1.0f)
		this->grid[top][midPointH] = (this->grid[top][left] + this->grid[top][right]) / 2;

	if (this->grid[bottom][midPointH] == -1.0f)
		this->grid[bottom][midPointH] = (this->grid[bottom][left] + this->grid[bottom][right]) / 2;

	if (this->grid[midPointV][left] == -1.0f)
		this->grid[midPointV][left] = (this->grid[top][left] + this->grid[bottom][left]) / 2;

	if (this->grid[midPointV][right] == -1.0f)
		this->grid[midPointV][right] = (this->grid[top][right] + this->grid[bottom][right]) / 2;


	if (this->grid[midPointV][midPointH] == -1.0f)
		this->grid[midPointV][midPointH] = (this->grid[top][left] + this->grid[top][right] + this->grid[bottom][left] + this->grid[bottom][right]) / 4 + randVal(left - right + 1);

	fractalRecurse(left, midPointH, midPointV, bottom);
	fractalRecurse(left, midPointH, top, midPointV);
	fractalRecurse(midPointH, right, top, midPointV);
	fractalRecurse(midPointH, right, midPointV, bottom);
}

void DEMGenerator::generateGrid(int width) {
	this->distribution = normal_distribution<float>(0.0, this->stdDev);

	fractalRecurse(0, width - 1, 0, width - 1);

	for (int i = 0; i < this->numSmooths; i++)
		this->smooth(this->grid);
}

string DEMGenerator::createGridFile() {
	generateGrid(this->gridWidth);

	ofstream outFile;

	outFile.open(outFileName.c_str());

	if (!outFile) {
		cout << "File " << outFileName << " doesn't exist" << endl;
		exit(1);
	}

	outFile << "ncols ";
	outFile << this->gridWidth;
	outFile << endl;

	outFile << "nrows ";
	outFile << this->gridWidth;
	outFile << endl;

	outFile << "xllcorner ";
	outFile << "0";
	outFile << endl;

	outFile << "yllcorner ";
	outFile << "0";
	outFile << endl;

	outFile << "cellsize ";
	outFile << this->cellSize;
	outFile << endl;

	for (int i = 0; i < this->gridWidth; i++) {
		for (int j = 0; j < this->gridWidth; j++) {
			outFile << grid[i][j] << " ";
		}
		outFile << endl;
	}

	return outFileName;
}

void DEMGenerator::display() {
	glDisable(GL_LIGHTING);
	int offset = this->gridWidth * this->cellSize / 2;

	glColor3f(0.75, 0.75, 0.75);
	glBegin(GL_LINES);
	for (float i = -offset; i <= offset; i += this->cellSize) {
		glVertex3f(i, 0, -offset);
		glVertex3f(i, 0, offset);
		glVertex3f(-offset, 0, i);
		glVertex3f(offset, 0, i);
	}
	glEnd();

	glBegin(GL_LINES);
	int x, y, z;
	for (int i = 0; i < this->gridWidth; i++) {
		for (int j = 0; j < this->gridWidth; j++) {
			if (this->grid[i][j] == -1.0f) continue; // Sentinel value for un-defined point

			else if (this->grid[i][j] == 0.0f) { // Sentinel value for lake
				// cout << "Lake at: " << i << ", " << j << endl;
				glColor3f(0.0, 0.0, 1.0);
				glEnd();
				glBegin(GL_QUADS);

				x = i - offset;
				z = j - offset;
				y = this->grid[i][j];
				glVertex3f(x-0.5, 0.1, z-0.5);
				glVertex3f(x+0.5, 0.1, z-0.5);
				glVertex3f(x+0.5, 0.1, z+0.5);
				glVertex3f(x-0.5, 0.1, z+0.5);

				glEnd();
				glBegin(GL_LINES);
			}
			else { // User defined point
				glColor3f(0.0, 1.0, 0.0);
				x = i - offset;
				z = j - offset;
				y = this->grid[i][j];
				glVertex3f(x, 0, z);
				glVertex3f(x, y, z);
			}
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void DEMGenerator::convert(int screenX, int screenY, int* indexX, int* worldY, int* indexZ) {

	GLint viewport[4];						// Where The Viewport Values Will Be Stored
	GLdouble projection[16];				// Where The 16 Doubles Of The Projection Matrix Are To Be Stored
	GLdouble modelview[16];					// Where The 16 Doubles Of The Modelview Matrix Are To Be Stored
	GLfloat windowX, windowY;

	glGetIntegerv(GL_VIEWPORT, viewport);           // Retrieves The Viewport Values (X, Y, Width, Height)
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);       // Retrieve The Modelview Matrix
	glGetDoublev(GL_PROJECTION_MATRIX, projection);     // Retrieve The Projection Matrix

	windowX = screenX;
	windowY = viewport[3] - screenY;

	GLdouble pos1X, pos1Y, pos1Z;
	GLdouble pos2X, pos2Y, pos2Z;

	gluUnProject( windowX, windowY, 0.0, modelview, projection, viewport, &pos1X, &pos1Y, &pos1Z);
	gluUnProject( windowX, windowY, 1.0, modelview, projection, viewport, &pos2X, &pos2Y, &pos2Z);

	Vec3f directionVec(pos1X - pos2X, pos1Y - pos2Y, pos1Z - pos2Z);
	Vec3f directionOrigin(pos1X, pos1Y, pos1Z);

	float t = -directionOrigin.y / directionVec.y;

	float x = directionOrigin.x + (t * directionVec.x);
	float z = directionOrigin.z + (t * directionVec.z);

	float offset = this->gridWidth * this->cellSize / 2;

	x += offset;
	z += offset;

	x /= this->cellSize;
	z /= this->cellSize;

	(*indexX) = x;
	(*worldY) = 0;
	(*indexZ) = z;
}

void DEMGenerator::handleClick(int button, int state, int x, int y) {

	if ((button == 3 or button == 4) and state == GLUT_DOWN) { // 3 is mouse wheel up, 4 is mouse wheel down
		int indexX, worldY, indexZ;

		this->convert(x, y, &indexX, &worldY, &indexZ);

		if (indexX >= this->gridWidth) indexX = this->gridWidth - 1;
		else if (indexX < 0) indexX = 0;
		if (indexZ >= this->gridWidth) indexZ = this->gridWidth - 1;
		else if (indexZ < 0) indexZ = 0;

		if (this->grid[indexX][indexZ] == -1.0f)
			this->grid[indexX][indexZ] = this->incrAmount;
		else
			this->grid[indexX][indexZ] += this->incrAmount;
	}
}

void DEMGenerator::addLake(int x, int y, int size) {
	int indexX, worldY, indexZ;

	this->convert(x, y, &indexX, &worldY, &indexZ);

	for (int i = -size; i <= size; i++) {
		for (int j = -size; j <= size; j++) {
			if ((i == -size and j == -size) or (i == -size and j == size) or (i == size and j == -size) or (i == size and j == size)) continue;
			
			int x = i + indexX;
			int y = j + indexZ;

			if (x >= this->gridWidth) x = this->gridWidth - 1;
			else if (x < 0) x = 0;
			if (y >= this->gridWidth) y = this->gridWidth - 1;
			else if (y < 0) y = 0;

			this->grid[x][y] = 0.0f;
		}
	}
}

// -------------------------------------------------------------------------------------------
