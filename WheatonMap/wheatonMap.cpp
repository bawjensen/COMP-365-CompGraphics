#include <GL/glut.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "textures.cpp"
#include "classes.cpp"
#include "constants.cpp"

using namespace std;

static vector<Building> buildingList;
static Building dummyBuilding;
static int userWindowWidth = 600;
static int userWindowHeight = 600;
static int viewportBaseOffsetX = 0; // 
static int viewportBaseOffsetY = 0;
static float currentScale = 1.0;
static float scaleSpeed = 1.2;
static CoordPoint clickDown(0, 0);
static vector<string> selectedBuildings;
static int viewportPanOffsetX = 0;
static int viewportPanOffsetY = 0;
static int activePanShiftX = 0;
static int activePanShiftY = 0;

static bool highlightChapel = false;
static bool highlightDorms = false;
static bool highlightFieldHouse = false;
static bool highlightLibrary = false;
static bool highlightMarsScience = false;
static bool roadsOn = true;
static bool labelsOn = false;

CoordPoint convertCoordinates(int x, int y, bool invertY=false) {
	if (invertY)
		y = userWindowHeight - y; // Invert Y (drawing and clicking have different 0,0 locations)

	int fullXShift = viewportBaseOffsetX + viewportPanOffsetX + activePanShiftX;
	int fullYShift = viewportBaseOffsetY + viewportPanOffsetY + activePanShiftY;

	x -= fullXShift;
	y -= fullYShift;

	x /= currentScale;
	y /= currentScale;

	return CoordPoint(x, y);
}

CoordPoint convertCoordinates(CoordPoint point, bool invertY=false) {
	return convertCoordinates(point.getX(), point.getY(), invertY);
}

void loadBuildings() {
	ifstream inFile;

	inFile.open(filename.c_str());

	if (!inFile) {
		cout << "File didn't exist" << endl;
		exit(1);
	}

	int x, y, buildingType;
	char trash;
	string label;

	string line;
	while (getline(inFile, line)) {
		Building newBuilding;
		if (line[0] == '#') {
			continue;
		}
		stringstream converter(line);

		converter >> label;
		converter >> buildingType;
		newBuilding.setLabel(label);
		newBuilding.setType(buildingType);

		while (converter >> x >> trash >> y) {
			newBuilding.addCorner(x, y);
		}

		newBuilding.calculateAABB();
		buildingList.push_back(newBuilding);
	}

	if (buildingList.size() < 1) {
		buildingList.push_back(dummyBuilding);
	}
}

void saveBuildings() {
	ofstream outFile;
	outFile.open(filename.c_str());

	if (!outFile) {
		cout << "File out didn't exist" << endl;
		exit(1);
	}

	for (vector<Building>::iterator it = buildingList.begin(); it < buildingList.end(); it++) {
		outFile << it->writeBuilding() << endl;
	}
}

void drawBuildings() {
	// cout << "Drawing " << buildingList.size() << " buildings." << endl;
	for (vector<Building>::iterator it = buildingList.begin(); it < buildingList.end(); it++) {
		if ( (it->getType() == BUILDING_DORM) && highlightDorms ) it->draw(true);
		else if ( (it->getLabel() == "ColeChapel") && highlightChapel ) it->draw(true);
		else if ( (it->getLabel() == "OldfieldHouse") && highlightFieldHouse ) it->draw(true);
		else if ( (it->getLabel() == "Library") && highlightLibrary ) it->draw(true);
		else if ( (it->getLabel() == "MarsScience") && highlightMarsScience ) it->draw(true);
		else if ( (it->getType() == BUILDING_ROAD) && !roadsOn );
		else {
			it->draw();
		}
	}
}

void startNewBuilding() {
	cout << "Starting building, resetting: " << endl;
	buildingList.push_back(dummyBuilding);
}

void eraseAllBuildings() {
	buildingList.clear();
	buildingList.push_back(dummyBuilding);
}

void activeBuildingAddCorner(int x, int y) {
	CoordPoint convertedPoint = convertCoordinates(x, y, true);

	x = convertedPoint.getX();
	y = convertedPoint.getY();

	buildingList[ buildingList.size()-1 ].addCorner(x, y);
}

vector<string> buildingsClicked(CoordPoint point) {
	vector<string> buildingLabels;
	for (vector<Building>::iterator it = buildingList.begin(); it < buildingList.end(); it++) {
		if (it->contains(point)) {
			buildingLabels.push_back( it->getLabel() );
		}
	}

	return buildingLabels;
}

void keyUpCallback(unsigned char key, int x, int y) { // Note: x and y are from cursor
	switch(key) {
	}
}

void keyDownCallback(unsigned char key, int x, int y) { // Note: x and y are from cursor
	// switch(key) {
	// 	case 'a':	activeBuildingAddCorner(x, y);
	// 				break;
	// 	case 'd':	startNewBuilding();
	// 				break;
	// 	case KEY_ESCAPE:
	// 	case 'q': 	saveBuildings();
	// 				exit(1);
	// }
	switch(key) {
		case 'c':	highlightChapel = !highlightChapel;
					break;
		case 'd':	highlightDorms = !highlightDorms;
					break;
		case 'f':	highlightFieldHouse = !highlightFieldHouse;
					break;
		case 'l':	highlightLibrary = !highlightLibrary;
					break;
		case 's':	highlightMarsScience = !highlightMarsScience;
					break;
		case 'R':	roadsOn = !roadsOn;
					break;
		case 'L':	labelsOn = !labelsOn;
					break;
		case KEY_ESCAPE:
		case 'q': 	exit(1);
	}
}

void mouseCallback(int button, int state, int x, int y) {
	if (state == GLUT_UP) { // Mouse Up
		if (button == GLUT_LEFT_BUTTON) {
			viewportPanOffsetX += activePanShiftX;
			viewportPanOffsetY += activePanShiftY;
			activePanShiftX = 0;
			activePanShiftY = 0;
		}
	}
	else { // Mouse Down
		if (button == GLUT_LEFT_BUTTON) {
			clickDown = CoordPoint(x, userWindowHeight - y);
			selectedBuildings = buildingsClicked( convertCoordinates(x, y, true) );
		}
		else if (button == MOUSE_SCROLL_UP) {
			currentScale *= scaleSpeed;
		}
		else if (button == MOUSE_SCROLL_DOWN) {
			currentScale /= scaleSpeed;
			currentScale = currentScale < 0.1 ? 0.1 : currentScale; // Provide lower limit for scale
		}
	}
}

void mouseMotionCallback(int x, int y) {
	y = userWindowHeight - y; // Invert y (coords start upper left, instead of lower left)

	// viewportPanOffsetX = x - userWindowWidth / 2;
	// viewportPanOffsetY = y - userWindowHeight / 2;
	
	activePanShiftX = x - clickDown.getX();
	activePanShiftY = y - clickDown.getY();

	// cout << "Original click at: " << clickDown.getX() << ", " << clickDown.getY() << endl;

	// cout << "Shifted right: " << activePanShiftX << endl;
	// cout << "Shifted up: " << activePanShiftY << endl;

	// cout << "Perm shifted right: " << viewportPanOffsetX << endl;
	// cout << "Perm shifted up: " << viewportPanOffsetY << endl;
}

void menuCallback(int value) {
	// if (value == 1) {
	// 	string output = "Hey!";
	// 	cout << "Drawing at: " << lastClickX << ", " << lastClickY << endl;
	// 	glRasterPos2i(lastClickX, windowHeight-lastClickY);
	// 	for (int i = 0; i < output.length(); i++) {
	// 		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, output[i]);
	// 	}
	// }
}

void initMenu() {
	glutCreateMenu(menuCallback); // register callback function
	glutAddMenuEntry("Menu", 0);
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Hello", 1);
	glutAddMenuEntry("Goodbye", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init() {
	glClearColor(0.875, 0.875, 1.0, 1.0);
	// glClearColor(0.0, 0.0, 0.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, mapWidth, 0.0, mapHeight);

	// Initialize buildings.
	loadBuildings();

	// Initialize menu.
	// initMenu();

	// Create texture index array.
	initTextures();
	// Load texture from BMP.
	loadExternalTextures();
}

void resize(int w, int h) {
	userWindowWidth = w;
	userWindowHeight = h;
}

void shiftAndScaleViewport() {
	viewportBaseOffsetX = (userWindowWidth - mapWidth * currentScale) / 2;
	viewportBaseOffsetY = (userWindowHeight - mapHeight * currentScale) / 2;

	int fullXShift = viewportBaseOffsetX + viewportPanOffsetX + activePanShiftX;
	int fullYShift = viewportBaseOffsetY + viewportPanOffsetY + activePanShiftY;

	glViewport((GLsizei)fullXShift,
			   (GLsizei)fullYShift,
			   (GLsizei)mapWidth * currentScale,
			   (GLsizei)mapHeight * currentScale );
}

void drawGrid() {
	// glColor3f(1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	int gridSize = 50;

	for (int i = 1; i < mapHeight; i += gridSize) {
		glBegin(GL_LINES);
			glVertex2i(0, i);
			glVertex2i(mapWidth, i);
		glEnd();
	}
	for (int i = 1; i < mapWidth; i += gridSize) {
		glBegin(GL_LINES);
			glVertex2i(i, 0);
			glVertex2i(i, mapHeight);
		glEnd();
	}
}

void textDisplay(string displayString, CoordPoint point) {
	// glColor3f(0.0, 0.0, 0.0);
	int yPos = point.getX();
	int xPos = point.getY();

	glRasterPos2i (xPos, yPos);
	for (string::iterator it = displayString.begin(); it != displayString.end(); it++) {
		glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, *it);
	}
}

void textDisplay(vector<string> strVect, CoordPoint point) {
	glColor3f(0.0, 0.0, 0.0);

	int xPos = point.getX();
	int yPos = point.getY();

	for (vector<string>::iterator it = strVect.begin(); it < strVect.end(); it++) {
		glRasterPos2i (xPos, yPos);
		for (string::iterator it2 = it->begin(); it2 != it->end(); it2++) {
			glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, *it2);
		}
		yPos -= 20;
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	shiftAndScaleViewport();

	// drawTexturedMap();
	// drawGrid();
	drawBuildings();
	textDisplay(selectedBuildings, convertCoordinates(clickDown));

	glFlush();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	// Initialize GLUT.
	glutInit(&argc, argv);
	// Set display mode as single-buffered and RGB color.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	// Set OpenGL window size.
	glutInitWindowSize(userWindowWidth, userWindowHeight); // Scale factor is scaling the map to fit screen 
	// Set OpenGL window position.
	glutInitWindowPosition(100, 100); 
	// Create OpenGL window with title.
	glutCreateWindow("Wheaton College Map");

	// Initialize.
	init();

	// Register key-up routine.
	glutKeyboardUpFunc(keyUpCallback);
	// Register key-down routine.
	glutKeyboardFunc(keyDownCallback);
	// Register mouse routine.
	glutMouseFunc(mouseCallback);
	// Register mouse movement routine.
	glutMotionFunc(mouseMotionCallback);
	// Register display routine.
	glutDisplayFunc(display);
	// Register reshape routine.
	glutReshapeFunc(resize);

	// Begin processing.
	glutMainLoop();

	return 0; 
}