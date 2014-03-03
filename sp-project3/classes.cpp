#include "classes.h"

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

void Vec3f::rotateXZ(float radians, float yRadians) {
	cout << "Currently at a " << xzAngle << " angle" << endl;
	this->xzAngle += yRadians;
	cout << "Currently at a " << xzAngle << " angle" << endl;
	float rotationArray1[4][4] = { 	{cos(-xzAngle),	0, 		-sin(-xzAngle), 0}, 
									{0, 			1, 		0, 				0},
									{sin(-xzAngle), 0, 		cos(-xzAngle), 	0},
									{0, 			0, 		0, 				1}
								};
	float rotationArray2[4][4] = { 	{1,		0, 				0, 				0}, 
									{0, 	cos(radians), 	sin(radians), 	0},
									{0, 	-sin(radians), 	cos(radians), 	0},
									{0, 	0, 				0, 				1}
								};
	float rotationArray3[4][4] = { 	{cos(xzAngle),	0, 		-sin(xzAngle), 	0}, 
									{0, 			1, 		0, 				0},
									{sin(xzAngle), 	0, 		cos(xzAngle), 	0},
									{0, 			0, 		0, 				1}
								};

	Matrix44f rMatrix_composite(rotationArray1);
	Matrix44f rMatrix1(rotationArray2);
	Matrix44f rMatrix2(rotationArray3);

	rMatrix_composite *= rMatrix1;
	rMatrix_composite *= rMatrix2;

	(*this) *= rMatrix_composite;
}

void Vec3f::rotateY(float radians) {
	float rotationArray[4][4] = { 	{cos(radians),	0, 		-sin(radians), 	0}, 
									{0, 			1, 		0, 				0},
									{sin(radians), 	0, 		cos(radians), 	0},
									{0, 			0, 		0, 				1}
								};

	Matrix44f rMatrix(rotationArray);
	(*this) *= rMatrix;
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
		// cout << "Doing a row of the first matrix: " << endl;
		for (int secondColumn = 0; secondColumn < 4; secondColumn++) {
			// cout << "Doing a column of the second matrix: " << endl;
			sum = 0;
			for (int secondRow = 0; secondRow < 4; secondRow++) {
				// cout << "Doing a column/row of the first/second matrix: " << endl;
				// cout << "Multiplying " << firstRow << "," << secondRow << " by " << secondRow << "," << secondColumn << " - aka: ";
				// cout << this->matrix[firstRow][secondRow] << " x " << other.matrix[secondRow][secondColumn] << " sum: " << sum << endl;
				sum += this->matrix[firstRow][secondRow] * other.matrix[secondRow][secondColumn];
			}
			tempMatrix.matrix[firstRow][secondColumn] = sum;
			// cout << "Storing " << sum << " into " << firstRow << "," << secondColumn << endl;
			// cout << endl;
		}
		// cout << endl;
	}
	// cout << "First: " << (*this) << endl;
	// cout << "Times second: " << other << endl;
	// cout << "Equals: " << tempMatrix << endl << endl << endl;

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

Camera::Camera() {
	this->pos = Vec3f(0.0f, 0.0f, 1.0f);
	this->isFocusing = true;
	this->viewDir = Vec3f(0.0f, 0.0f, -1.0f);
	this->focus = Vec3f(0.0f, 0.0f, 0.0f);
	this->angularScrollSpeed = 1 / 1000 * 2;
}

void Camera::setPos(float pX, float pY, float pZ) {
	this->pos = Vec3f(pX, pY, pZ);
}

void Camera::setViewDir(float vX, float vY, float vZ) {
	this->viewDir = Vec3f(vX, vY, vZ);
}

void Camera::setFocus(float fX, float fY, float fZ) {
	this->focus = Vec3f(fX, fY, fZ);
}

void Camera::rotate(float hAngle, float vAngle) {
	this->pos.rotateY(hAngle);


}

void Camera::handleClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) { // only start motion if the left button is pressed
		if (state == GLUT_UP) { // when the button is released
			this->panActive = false;
		}
		else  { // state = GLUT_DOWN
			this->clickOrigin = Vec3f(x, y, 0); // Ignore z for simplicity
			this->panActive = true;
		}
	}
}

void Camera::handleMovement(int x, int y) {
	if (panActive) { // this will only be true when the left button is down
		// update deltaHorizAngle
		this->deltaHorizAngle = -(this->clickOrigin.x - x) * this->angularScrollSpeed;
		// lastX = x;

		// update camera's position (rotation y-axis)
		// camPos.rotateY(deltaHorizAngle);

		// update deltaVertAngle
		this->deltaVertAngle = -(this->clickOrigin.y - y) * angularScrollSpeed;
		// lastY = y;

		this->rotate(deltaHorizAngle, deltaVertAngle);

		// update camera's position (rotation xz-axis)
		// camPos.rotateXZ(deltaVertAngle, deltaHorizAngle);
	}
}