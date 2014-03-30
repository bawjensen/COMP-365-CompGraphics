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

	return (*this);
}

Vec3f& Vec3f::operator*=(float multiplier) {
	this->x *= multiplier;
	this->y *= multiplier;
	this->z *= multiplier;

	return (*this);
}

Vec3f& Vec3f::operator=(const Vec3f& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;

	return (*this);
}

Vec3f Vec3f::operator-(const Vec3f& other) {
	return Vec3f(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vec3f Vec3f::operator+(const Vec3f& other) {
	return Vec3f(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vec3f Vec3f::operator*(const float scalar) const {
	return Vec3f(this->x * scalar, this->y * scalar, this->z * scalar);
}

Vec3f& Vec3f::operator+=(const Vec3f& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;

	return (*this);
}

Vec3f& Vec3f::operator-=(const Vec3f& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;

	return (*this);
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
	this->pos = Vec3f(0.0f, 100.0f, 0.0f); // Camera position
	this->origViewDir = Vec3f(-1.0f, 0.0f, 0.0f); // View direction
	this->viewDir = this->origViewDir; // View direction
	this->strafeVec = this->viewDir.rotateY(-M_PI / 2);

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
	float moveSpeedFactor = 2.0;
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

	this->viewDir = this->origViewDir.rotateZ(this->vertAngle).rotateY(this->horizAngle);
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

	this->xOffset = -this->cellSize * this->nRows / 2;
	this->yOffset = -this->cellSize * this->nCols / 2;

	this->greaterDimension = max(this->nCols, this->nRows) * this->cellSize;

	this->firstDelimiter = (this->highest - this->lowest) / 5;
	this->secondDelimiter = 4 * (this->highest - this->lowest) / 5;
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
	int xPos, zPos, index1, index2, tempX, tempZ;
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 1.0);

	for (int i = 0; i < (this->nRows - 1); i++) {
		for (int j = 0; j < this->nCols; j++) {
			xPos = (i - this->nRows / 2) * this->cellSize;
			zPos = (j - this->nCols / 2) * this->cellSize;

			if (j != this->nRows - 1) {
				for (int n = 0; n < 3; n++) {
					switch (n) {
						case 0:	index1 = i;
								index2 = j;
								tempX = xPos;
								tempZ = zPos;
								break;
						case 1:	index1 = i+1;
								index2 = j;
								tempX = xPos + this->cellSize;
								tempZ = zPos;
								break;
						case 2:	index1 = i;
								index2 = j+1;
								tempX = xPos;
								tempZ = zPos + this->cellSize;
								break;
					}

					float point = this->pointGrid[index1][index2];

					if (point >= this->lowest and point < this->firstDelimiter) {
						this->setGreen();
					}
					else if (point >= this->firstDelimiter and point < this->secondDelimiter) {
						this->setGray();
					}
					else {
						this->setWhite();
					}

					glVertex3f(tempX, point, tempZ);
				}
			}

			if (j != 0) {
				for (int n = 0; n < 3; n++) {
					switch (n) {
						case 0:	index1 = i;
								index2 = j;
								tempX = xPos;
								tempZ = zPos;
								break;
						case 1:	index1 = i+1;
								index2 = j-1;
								tempX = xPos + this->cellSize;
								tempZ = zPos - this->cellSize;
								break;
						case 2:	index1 = i+1;
								index2 = j;
								tempX = xPos + this->cellSize;
								tempZ = zPos;
								break;
					}

					float point = this->pointGrid[index1][index2];

					if (point >= this->lowest and point < this->firstDelimiter) {
						this->setGreen();
					}
					else if (point >= this->firstDelimiter and point < this->secondDelimiter) {
						this->setGray();
					}
					else {
						this->setWhite();
					}

					glVertex3f(tempX, point, tempZ);
				}
			}
		}
	}
	glEnd();
}

// -------------------------------------------------------------------------------------------

PlantGrammar::PlantGrammar() {

}

void PlantGrammar::loadFromFile(string filename) {
	ifstream inFile;

	inFile.open(filename.c_str());

	if (!inFile) {
		cout << "Grammar file " << filename << " doesn't exist." << endl;
		exit(1);
	}

	char symbol;
	char trash;
	float value;
	string start;
	string line, replacement;

	inFile >> trash;
	inFile >> value;

	this->depthLimit = value;

	inFile >> trash;
	inFile >> value;

	this->angle = value;

	inFile >> trash;
	inFile >> value;

	this->length = value;

	inFile >> trash;
	inFile >> start;

	this->start = start;

	getline(inFile, line); // Trash the remaining remnants of the previous line

	getline(inFile, line); // Start the line-getting
	while (inFile) {
		symbol = line[0];

		replacement = line.substr(3, 999);

		this->grammarMap[symbol].push_back(replacement);

		getline(inFile, line);
	}
}

// -------------------------------------------------------------------------------------------

Plant::Plant() {
	
}

Plant::Plant(int x, int y, int z, string plantString, float angle, float length) {
	this->startPos = Vec3f(x, y, z);
	this->plantString = plantString;
	this->angle = angle;
	this->length = length;
}

void Plant::rotateX(string followingSubstr) {
	for (int i = 0; i < followingSubstr.length(); i++) {
		if (followingSubstr[i] == '+') {
			glRotatef(this->angle, 1.0, 0.0, 0.0);
		}
		else if (followingSubstr[i] == '-') {
			glRotatef(-this->angle, 1.0, 0.0, 0.0);
		}
		else {
			break;
		}
	}
}

void Plant::rotateY(string followingSubstr) {
	for (int i = 0; i < followingSubstr.length(); i++) {
		if (followingSubstr[i] == '+') {
			glRotatef(this->angle, 0.0, 1.0, 0.0);
		}
		else if (followingSubstr[i] == '-') {
			glRotatef(-this->angle, 0.0, 1.0, 0.0);
		}
		else {
			break;
		}
	}
}

void Plant::display() {
	glColor3f(0.55, 0.275, 0.1);
	glPushMatrix();
	glTranslatef(this->startPos.x, this->startPos.y, this->startPos.z);
	glScalef(0.1, 0.1, 0.1);
	for (int i = 0; i < this->plantString.length(); i++) {
		switch(plantString[i]) {
			case 'B':	this->drawBranch();
						break;
			case 'F':	this->drawFlower();
						break;
			case 'L':	this->drawLeaf();
						break;
			case 'X':	this->rotateX(plantString.substr(i+1, 6));
						break;
			case 'Y':	this->rotateY(plantString.substr(i+1, 6));
						break;
			case '[':	glPushMatrix();
						glScalef(0.5, 0.5, 0.5);
						break;
			case ']':	glPopMatrix();
						break;
		}
	}
	glPopMatrix();
}

void Plant::drawBranch() {
	glPushMatrix();

	glScalef(1.0, this->length, 1.0);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 1, 1, 1, 20, 1);

	glPopMatrix();

	glTranslatef(0.0, this->length, 0.0);
}

void Plant::drawFlower() {
	glPushMatrix();

	gluSphere(gluNewQuadric(), 2, 20, 20);

	glPopMatrix();
}

void Plant::drawLeaf() {
	glPushMatrix();

	glTranslatef(0.0, 0.5, 0.0);
	glutSolidTeapot(2.0);

	glPopMatrix();
}

// -------------------------------------------------------------------------------------------

PlantLandscape::PlantLandscape() {
	this->nGrammars = 0;

	srand(time(NULL));
}

void PlantLandscape::display() {
	// for (int i = 0; i < this->nPlants; i++) {
	for (vector<Plant>::iterator it = this->plantVec.begin(); it != this->plantVec.end(); ++it) {
		it->display();
	}
}

void PlantLandscape::loadGrammar(string filename) {

	this->grammarArray[this->nGrammars++].loadFromFile(filename);

	this->nGrammars++;
}

string PlantLandscape::generatePlantString(int type) {
	PlantGrammar grammar = this->grammarArray[type];

	int limit = grammar.depthLimit;
	float angle = grammar.angle;

	string plantString = grammar.start;
	string tempString = "";

	for (int n = 0; n < limit; n++) {
		for (int i = 0; i < plantString.length(); i++) {
			char plantChar = plantString[i];

			if (grammar.grammarMap.find(plantChar) == grammar.grammarMap.end()) {
				tempString += plantChar;
			}
			else {
				vector<string> rules = grammar.grammarMap[plantChar];
				int randomIndex = rand() % rules.size();
				string replacement = rules[randomIndex];
				tempString += replacement;
			}
		}

		plantString = tempString;
		tempString = "";
	}

	return plantString;
}

void PlantLandscape::addPlant(int x, int y, int z, int type) {
	string plantString = this->generatePlantString(type);
	float angle = this->grammarArray[type].angle;
	float length = this->grammarArray[type].length;
	
	this->plantVec.push_back(Plant(x, y, z, plantString, angle, length));
}

void PlantLandscape::handleClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON and state == GLUT_DOWN) { // only start motion if the left button is pressed
		this->addPlant(0, 0, 0, 0);
	}
}

// -------------------------------------------------------------------------------------------

Minimap::Minimap() {

}

Minimap::Minimap(int w, int h, int uRX, int uRY, Ground* gP, PlantLandscape* pLP) {
	this->width = w;
	this->height = h;

	this->upperRightX = uRX;
	this->upperRightY = uRY;

	this->right = this->upperRightX;
	this->left = this->upperRightX - this->width;

	this->top = upperRightY;
	this->bottom = upperRightY - this->height;

	this->groundPointer = gP;
	this->plantLandPointer = pLP;
}

void Minimap::display() {
	glColor3f(0, 1, 1);
	for (int i = left; i <= right; i += 10) {
		glBegin(GL_LINES);
			glVertex3f(i, bottom, 0);
			glVertex3f(i, top, 0);
		glEnd();
	}

	for (int i = bottom; i <= top; i += 10) {
		glBegin(GL_LINES);
			glVertex3f(left, i, 0);
			glVertex3f(right, i, 0);
		glEnd();
	}
}

void Minimap::displayIndicator() {
	glColor3f(1, 1, 0);
	glPointSize(10.0);
	glBegin(GL_POINTS);
		glVertex3f(indicator.x, indicator.y, indicator.z);
	glEnd();
}

void Minimap::handleClick(int button, int state, int x, int y) {
	y = upperRightY - y;

	if (x >= left and x <= right and y >= bottom and y <= top and state == GLUT_DOWN) {
		this->plantLandPointer->addPlant(indicator.x, indicator.y - 2, indicator.z, 0);
	}
}

void Minimap::handleMovement(int x, int y) {
	y = upperRightY - y;

	if (x >= left and x <= right and y >= bottom and y <= top) {
		float newX = (float)(x - left) / 200;
		newX = 1.0 - newX; // Invert 
		newX *= groundPointer->nRows;

		float newY = (float)(y - bottom) / 200;
		newY *= groundPointer->nCols;

		int i = newX;
		int j = newY;

		float xDrawPos = (newX - (groundPointer->nRows / 2)) * groundPointer->cellSize;
		float yDrawPos = (newY - (groundPointer->nCols / 2)) * groundPointer->cellSize;

		indicator = Vec3f(xDrawPos, groundPointer->pointGrid[i][j], yDrawPos);
	}
	else {
		indicator = Vec3f(0, 0, 0);
	}
}