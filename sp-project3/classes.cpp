#include "classes.h"

// -------------------------------------------------------------------------------------------

Vec3f::Vec3f() {
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->xzAngle = 0.0f;
}

Vec3f::Vec3f(float nX, float nY, float nZ) {
	this->x = nX;
	this->y = nY;
	this->z = nZ;
	this->xzAngle = 0.0f;
}

void Vec3f::set(float nX, float nY, float nZ) {
	this->x = nX;
	this->y = nY;
	this->z = nZ;
	this->xzAngle = 0.0f;
}

Vec3f Vec3f::rotateZ(float radians) {
	Vec3f tempVec = (*this);
	float rotationArray[4][4] = { 	{cos(radians),	sin(radians), 	0, 	0}, 
									{-sin(radians), cos(radians), 	0, 	0},
									{0, 			0,				1, 	0},
									{0, 			0,				0, 	1}
								};

	Matrix44f rMatrix(rotationArray);
	tempVec *= rMatrix;

	return tempVec;
}

Vec3f Vec3f::rotateY(float radians) {
	Vec3f tempVec = (*this);
	float rotationArray[4][4] = { 	{cos(radians),	0, 		-sin(radians), 	0}, 
									{0, 			1, 		0, 				0},
									{sin(radians), 	0, 		cos(radians), 	0},
									{0, 			0, 		0, 				1}
								};

	Matrix44f rMatrix(rotationArray);
	tempVec *= rMatrix;

	return tempVec;
}

Vec3f& Vec3f::operator*=(Matrix44f matrix) {
	float vec4[4] = { this->x, this->y, this->z, 1 };
	float tempVec[3] = { 0, 0, 0 };

	for (int i = 0; i < 4; i++) {
		float sum = 0;
		for (int j = 0; j < 4; j++) {
			 sum += (*this)[j] * matrix.matrix[i][j];
		}
		tempVec[i] = sum;
	}

	this->x = tempVec[0];
	this->y = tempVec[1];
	this->z = tempVec[2];
}

Vec3f& Vec3f::operator=(const Vec3f& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

float Vec3f::operator[](int i) {
	if (i == 0) return this->x;
	if (i == 1) return this->y;
	if (i == 2) return this->z;
}

ostream& operator<<(ostream& co, Vec3f& vec) {
	co << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return co;
}

// -------------------------------------------------------------------------------------------

Matrix44f::Matrix44f() { // Creates an identity matrix
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				this->matrix[i][j] = 1;
			}
			else {
				this->matrix[i][j] = 0;
			}
		}
	}
}

Matrix44f::Matrix44f(float array[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->matrix[i][j] = array[i][j];
		}
	}
}

Matrix44f& Matrix44f::operator*=(Matrix44f other) {
	Matrix44f tempMatrix;
	float sum;

	for (int firstRow = 0; firstRow < 4; firstRow++) {
		for (int secondColumn = 0; secondColumn < 4; secondColumn++) {
			sum = 0;
			for (int secondRow = 0; secondRow < 4; secondRow++) {
				sum += this->matrix[firstRow][secondRow] * other.matrix[secondRow][secondColumn];
			}
			tempMatrix.matrix[firstRow][secondColumn] = sum;
		}
	}
	(*this) = tempMatrix;

	return *this;
}

Matrix44f& Matrix44f::operator=(const Matrix44f& other) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->matrix[i][j] = other.matrix[i][j];
		}
	}
}

ostream& operator<<(ostream& co, Matrix44f& matrix) {
	co << "[";
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			co << matrix.matrix[i][j] << ", ";
		}
		co << endl;
	}
	co << "]";
	return co;
}

// -------------------------------------------------------------------------------------------

Camera::Camera() {
	this->pos = Vec3f(0.0f, 0.0f, 1.0f); // Camera position
	this->viewDir = Vec3f(0.0f, 0.0f, -1.0f); // View direction

	this->isFocusing = true; // Whether or not the camera is "viewing" or "focusing"
	this->focus = Vec3f(0.0f, 0.0f, 0.0f); // Where the camera is focusing

	this->angularScrollSpeed = 1 / (float)200; // Rotation speed of the camera
	this->panActive = false; // Whether or not the camera is panning (via the mouse)
}

void Camera::setPos(float pX, float pY, float pZ) {
	this->origPos = Vec3f(pX, pY, pZ);
	this->pos = this->origPos;
	this->rotationRadius = max(pX, pZ);
	this->depthOfView = 2 * this->rotationRadius;
	// cout << "Creating camera at: " << pX << "," << pY << "," << pZ << " at radius " << this->rotationRadius << endl;
}

void Camera::setViewDir(float vX, float vY, float vZ) {
	this->viewDir = Vec3f(vX, vY, vZ);
}

void Camera::setFocus(float fX, float fY, float fZ) {
	this->focus = Vec3f(fX, fY, fZ);
}

void Camera::setRotationRadius(int r) {
	// cout << "Old:new ratio: " << ((float)r / this->rotationRadius) << endl;
	float radiusRatio = (float)r / this->rotationRadius;

	this->origPos.x *= radiusRatio;
	this->origPos.y *= radiusRatio;
	this->origPos.z *= radiusRatio;

	this->pos.x *= radiusRatio;
	this->pos.y *= radiusRatio;
	this->pos.z *= radiusRatio;

	this->rotationRadius = r;
	this->depthOfView = 2 * this->rotationRadius;

	// cout << "Setting camera at: " << this->pos.x << "," << this->pos.y << "," << this->pos.z << " at radius " << this->rotationRadius << endl;
}

void Camera::rotateTo(float hAngle, float vAngle) {
	this->horizAngle = hAngle;
	this->vertAngle = vAngle;

	this->pos = this->origPos.rotateY(this->horizAngle);
}

void Camera::rotate(float hAngle, float vAngle) {
	this->horizAngle += hAngle;
	this->vertAngle += vAngle;

	this->pos = this->origPos.rotateZ(this->vertAngle).rotateY(this->horizAngle);
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

Spline::Spline() {
	this->pArray = NULL;
}

Spline::~Spline() {
	// if (this->pArray != NULL)
		// cout << "pArray: " << this->pArray << endl;
		// delete[] this->pArray;
}

void Spline::create(int mode, float array[], int length, int splineRow, int cellSize, bool crossWays) {
	switch(mode) {
		case SplineGrid::MODE_KNOTS: this->type = Spline::TYPE_KNOT;
			break;
		case SplineGrid::MODE_LINEAR: this->type = Spline::TYPE_LINEAR;
			break;
		case SplineGrid::MODE_QUADRATIC: this->type = Spline::TYPE_QUADRATIC;
			break;
	}

	this->length = length;

	if (this->pArray)
		delete[] pArray;

	this->pArray = new Vec3f[this->length];
	for (int i = 0; i < length; i++) {
		if (crossWays)
			this->pArray[i] = Vec3f((i - length / 2) * cellSize, array[i], splineRow * cellSize);
		else
			this->pArray[i] = Vec3f(splineRow * cellSize, array[i], (i - length / 2) * cellSize);
	}

	cout << "Created spline from " << this->pArray[0] << " to " << this->pArray[length - 1] << endl;
}

void Spline::display(float heightFactor) {
	if (this->type == Spline::TYPE_KNOT)
		glBegin(GL_POINTS);
	else if (this->type == Spline::TYPE_LINEAR)
		glBegin(GL_LINE_STRIP);

	for (int i = 0; i < this->length; i++) {
		// cout << "Drawing point at: " << pArray[i] << endl;
		glVertex3f(pArray[i].x, pArray[i].y * heightFactor, pArray[i].z);
	}

	glEnd();
}

// -------------------------------------------------------------------------------------------

SplineGrid::SplineGrid() {
	this->splineVectorArray = new vector<Spline>[SplineGrid::NUM_MODES];

	this->mode = SplineGrid::MODE_KNOTS;

	this->elevFactor = 1;
}

SplineGrid::~SplineGrid() {
	if (this->dataArray)
		delete[] this->dataArray;
}

void SplineGrid::setMode(int nMode) {
	if (!initialized[nMode]) {
		this->initialize(nMode);
	}
	this->mode = nMode;
}

void SplineGrid::changeGridElevation(float factor) {
	this->elevFactor *= factor;
	if (this->elevFactor == 0) {
		this->elevFactor = 1;
	}
}

void SplineGrid::readFromESRIFile(string filename) {
	if (this->dataArray) {
		for (int row = 0; row < nRows; row++) {
			if (this->dataArray[row])
				delete[] this->dataArray[row];
		}
		delete[] this->dataArray;
	}

	ifstream inFile;

	inFile.open(filename.c_str());

	if (!inFile) {
		cout << "File " << filename << " didn't exist" << endl;
		exit(1);
	}

	string labelTrash;
	float labelValue;

	inFile >> labelTrash >> labelValue;
	this->nCols = (int)labelValue;

	inFile >> labelTrash >> labelValue;
	this->nRows = (int)labelValue;

	inFile >> labelTrash >> labelValue;
	this->xllCorner = labelValue;

	inFile >> labelTrash >> labelValue;
	this->yllCorner = labelValue;

	inFile >> labelTrash >> labelValue;
	this->cellSize = labelValue;

	this->dataArray = new float*[nRows];
	for (int row = 0; row < nRows; row++) {
		this->dataArray[row] = new float[nCols];
	}

	float cellData;
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			inFile >> cellData;
			if (cellData > highest) highest = cellData;
			if (cellData < lowest) lowest = cellData;
			this->dataArray[i][j] = cellData;
		}
	}

	this->xOffset = -this->cellSize * this->nRows / 2;
	this->yOffset = -this->cellSize * this->nCols / 2;

	this->greaterDimension = max(this->nCols, this->nRows) * this->cellSize;

	this->initialize(this->mode);
}

void SplineGrid::initialize(int mode) {
	// cout << "Mode: " << mode << " is not initialized. Fixing." << endl;
	for (int i = 0; i < this->nRows; i++) {
		this->splineVectorArray[mode].push_back(Spline());
		this->splineVectorArray[mode].back().create(mode, this->dataArray[i], this->nCols, i - (this->nRows / 2), this->cellSize);
	}

	if (mode == SplineGrid::MODE_LINEAR or mode == SplineGrid::MODE_QUADRATIC) {
		float* tempArray = new float[this->nRows];
		for (int j = 0; j < this->nCols; j++) {
			for (int i = 0; i < this->nRows; i++) {
				tempArray[i] = this->dataArray[i][j];
			}
			this->splineVectorArray[mode].push_back(Spline());
			this->splineVectorArray[mode].back().create(mode, tempArray, this->nRows, j - (this->nCols / 2), this->cellSize, true);
		}
	}

	// cout << "Finished creating spline of size: " << this->splineVectorArray[mode].size() << endl;

	this->initialized[mode] = true;
}

void SplineGrid::display() {
	int x, y, z;

	glColor3f(0.6f, 0.83f, 0.6f);

	if (!this->initialized[this->mode]) {
		this->initialize(this->mode);
	}

	for (vector<Spline>::iterator it = this->splineVectorArray[this->mode].begin(); it != this->splineVectorArray[this->mode].end(); it++) {
		it->display(this->elevFactor);
	}
}