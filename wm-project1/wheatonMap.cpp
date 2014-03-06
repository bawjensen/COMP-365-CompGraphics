#include <GL/glut.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "textures.h"
#include "classes.h"
#include "constants.h"

using namespace std;

static vector<Building> buildingList; // List of all the buildings
static Building dummyBuilding; // Building with which to populate the above list
static int userWindowWidth = 600; // Width of the user's window
static int userWindowHeight = 600; // Height of the user's window

static float currentScale = 1.75; // Scale for the viewport
static float scaleSpeed = 1.2; // Speed at which the map scales

static CoordPoint clickDown(0, 0); // Position of the click on mouse down
static bool panActive = false; // Whether or not the user is actively panning across the map
static int panSensitivity = 20; // Distance the pan movement must exceed before being displayed
static vector<string> selectedBuildings; // Buildings which have been selected by click for larger labelling

static int viewportBaseOffsetX = 0; // Offset width for the viewport within the user window
static int viewportBaseOffsetY = 0; // Offset height for the viewport within the user window
static int viewportPanOffsetX = 0; // Offset width for the viewport due to panning
static int viewportPanOffsetY = 0; // Offset height for the viewport due to panning
static int activePanShiftX = 0; // Offset width for the viewport due to the active pan
static int activePanShiftY = 0; // Offset height for the viewport due to the active pan
static int fullXShift = 0; // Full shift of the viewport in the x direction
static int fullYShift = 0; // Full shift of the viewport in the y direction

static bool highlightChapel = false; // Boolean for highlighted status of the chapel
static bool highlightDorms = false; // Boolean for highlighted status of the dorm
static bool highlightFieldHouse = false; // Boolean for highlighted status of the field house
static bool highlightLibrary = false; // Boolean for highlighted status of the library
static bool highlightMarsScience = false; // Boolean for highlighted status of the Mars Science Center
static bool roadsOn = true; // Boolean for display status of the roads
static bool labelsOn = false; // Boolean for display status of the labels

/*
Function to convert click-coordinates to draw-coordinates.
Precondition: x and y come in as points originating from upper left.
Postcondition: x and y are returned as attributes of a CoordPoint corresponding to the 
			   correct locations in the viewport for drawing.
*/
CoordPoint convertCoordinates(int x, int y, bool invertY=false) {
	if (invertY)
		y = userWindowHeight - y; // Invert Y (drawing and clicking have different 0,0 locations)

	x -= fullXShift; // Unshift the point
	y -= fullYShift; // Unshift the point

	x /= currentScale; // Unscale the point
	y /= currentScale; // Unscale the point

	return CoordPoint(x, y);
}


/*
Function to convert click-coordinates to draw-coordinates.
Precondition: x and y come in as attributes of a CoordPoint originating from upper left.
Postcondition: x and y are returned as attributes of a CoordPoint corresponding to the 
			   correct locations in the viewport for drawing.
*/
CoordPoint convertCoordinates(CoordPoint point, bool invertY=false) {
	return convertCoordinates(point.getX(), point.getY(), invertY); // Redirect to other function
}


/*
Function to load all buildings from a file (from the filename global) to the buildingList global variable.
Precondition: buildingList is empty and filename corresponds to a valid file.
Postcondition: buildingList is populated with the data contents of the file marked by the filename global
*/
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

/*
Funcion to write the contents of buildingList to the filename file.
Precondition: buildingList contains all of the data of the map's buildings and filename represents a valid file.
Postcondition: filename's file contains the formatted data of buildingList.
*/
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

/*
Function to draw all of the buildings in buildingList to the GLUT window context.
Precondition: buildingList contains the map's buildings to be drawn.
Postcondition: Those buildings have been rendered to the GLUT window context.
*/
void drawBuildings() {
	for (vector<Building>::iterator it = buildingList.begin(); it < buildingList.end(); it++) {
		if (( it->getType() == BUILDING_DORM && highlightDorms ) ||
			( it->getLabel() == "ColeChapel" && highlightChapel ) ||
			( it->getLabel() == "OldfieldHouse" && highlightFieldHouse ) ||
			( it->getLabel() == "Library" && highlightLibrary ) ||
			( it->getLabel() == "MarsScienceUpper" && highlightMarsScience )) it->draw(true, labelsOn, currentScale);
		else if ( it->getType() == BUILDING_ROAD && !roadsOn );
		else {
			it->draw(false, labelsOn, currentScale);
		}
	}
}

/*
Creation function for buildings - used in map's creation.
Precondition: dummyBuilding, buildingList globals have been initialized.
Postcondition: A new building has been inserted as the last element of buildingList.
*/
void startNewBuilding() {
	buildingList.push_back(dummyBuilding);
}

/*
Clears the entire contents of the buildingList global, clearing the map.
Precondition: dummyBuilding, buildingList globals have been initialized.
Postcondition: buildingList is empty of any buildings.
*/
void eraseAllBuildings() {
	buildingList.clear();
	buildingList.push_back(dummyBuilding);
}

/*
Adds a new corner to the last element of buildingList, the "active" building.
Precondition: buildlingList has at least one Building object.
Postcondition: The last element of the buildingList global has a new CoordPoint corner.
*/
void activeBuildingAddCorner(int x, int y) {
	CoordPoint convertedPoint = convertCoordinates(x, y, true);

	x = convertedPoint.getX();
	y = convertedPoint.getY();

	buildingList[ buildingList.size()-1 ].addCorner(x, y);
}

/*
Finds and returns all buildings intersected with a certain point.
Precondition: buildingList has been initialized.
Postcondition: buildingLabels is returned with the labels of all buildings around the given point.
*/
vector<string> buildingsClicked(CoordPoint point) {
	vector<string> buildingLabels;
	for (vector<Building>::iterator it = buildingList.begin(); it < buildingList.end(); it++) {
		if (it->contains(point)) {
			buildingLabels.push_back( it->getLabel() );
		}
	}

	return buildingLabels;
}

/*
Callback function for the keypress event.
Precondition: key pressed, as well as the cursor x and y are passed in from GLUT.
Postcondition: Various actions are performed based on which key was pressed.
*/
void keyDownCallback(unsigned char key, int x, int y) { // Note: x and y are from cursor
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

/*
Callback function for mouse events.
Precondition: button, state, x and y are passed based on the current state of the mouse.
Postcondition: Various actions are performed based on the location of the click, including
			   label display, panning and zooming.
*/
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
			panActive = false;
			selectedBuildings = buildingsClicked( convertCoordinates(x, y, true) );
		}
		else if (button == MOUSE_SCROLL_UP) {
			selectedBuildings.clear();
			currentScale *= scaleSpeed;
		}
		else if (button == MOUSE_SCROLL_DOWN) {
			selectedBuildings.clear();
			currentScale /= scaleSpeed;
			currentScale = currentScale < 0.1 ? 0.1 : currentScale; // Provide lower limit for scale
		}
	}
}

/*
Callback function for click'n'drag mouse movement.
Precondition: x and y are passed in based on the location of the mouse
Postcondition: Panning may be performed based on the state of certain global variables.
*/
void mouseMotionCallback(int x, int y) {
	y = userWindowHeight - y; // Invert y (coords start upper left, instead of lower left)
	
	activePanShiftX = x - clickDown.getX();
	activePanShiftY = y - clickDown.getY();

	if (abs(activePanShiftX) > panSensitivity || abs(activePanShiftY) > panSensitivity) {
		panActive = true;
		selectedBuildings.clear();
	}
}

/*
Callback function for resizing of the window.
Precondition: w and h are passed in based on the width and height of the user's window.
Postcondition: The two globals are updated based on the passed in arguments.
*/
void resize(int w, int h) {
	userWindowWidth = w;
	userWindowHeight = h;
}

/*
Function to handle the required shifts and resizes every time the window is redisplayed.
Precondition: All of the globals used have been intialized.
Postcondition: The contents of the globals are updated with the latest changes to
			   other global variables, and then the viewport is shifted accordingly.
*/
void shiftAndScaleViewport() {
	viewportBaseOffsetX = (userWindowWidth - mapWidth * currentScale) / 2;
	viewportBaseOffsetY = (userWindowHeight - mapHeight * currentScale) / 2;

	if (panActive) {
		fullXShift = viewportBaseOffsetX + viewportPanOffsetX + activePanShiftX;
		fullYShift = viewportBaseOffsetY + viewportPanOffsetY + activePanShiftY;
	}
	else {
		fullXShift = viewportBaseOffsetX + viewportPanOffsetX;
		fullYShift = viewportBaseOffsetY + viewportPanOffsetY;
	}

	glViewport((GLsizei)fullXShift,
			   (GLsizei)fullYShift,
			   (GLsizei)mapWidth * currentScale,
			   (GLsizei)mapHeight * currentScale );
}

/*
Function to take a string and display it, lower left corner at the CoordPoint.
Precondition: None.
Postcondition: The displayString is output to the GLUT context window at the x, y of the CoordPoint.
*/
void textDisplay(string displayString, CoordPoint point) {
	// glColor3f(0.0, 0.0, 0.0);
	int yPos = point.getX();
	int xPos = point.getY();

	glRasterPos2i (xPos, yPos);
	for (string::iterator it = displayString.begin(); it != displayString.end(); it++) {
		glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, *it);
	}
}

/*
Function to take a vector of strings and display it, lower left corner at the CoordPoint.
Precondition: None.
Postcondition: The contents of strVect have been output to the GLUT context window.
*/
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

/*
Display loop function for GLUT drawing.
Precondition: display() called by GLUT.
Postcondition: All of the output to the GLUT context window has been performed by subfunctions,
			   and a flag is set with glutPostRedisplay() for immediate redrawing.
*/
void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	shiftAndScaleViewport();

	drawBuildings();
	textDisplay(selectedBuildings, convertCoordinates(clickDown));

	glFlush();
	glutPostRedisplay();
}

/*
Initialization function for GLUT.
Precondition: None.
Postcondition: All of the OpenGL, and map-specific intialization has been performed with GLUT
			   and personally defined functions.
*/
void init() {
	glClearColor(0.875, 0.875, 1.0, 1.0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, mapWidth, 0.0, mapHeight);

	// Initialize buildings.
	loadBuildings();

	// Create texture index array.
	initTextures();
	// Load texture from BMP.
	loadExternalTextures();
}

/*
Stdio output instructions for the user to interact with this program.
Precondition: None.
Postcondition: Much output to stdio.
*/
void introInstructions() {
	cout << "Welcome to Wheaton College's Map!" << endl;
	cout << "Keyboard Commands:" << endl;
	cout << "\tc: Toggle highlighting of the Chapel." << endl;
	cout << "\td: Toggle highlighting of the Dorms." << endl;
	cout << "\tf: Toggle highlighting of the Field House." << endl;
	cout << "\tl: Toggle highlighting of the Library." << endl;
	cout << "\ts: Toggle highlighting of the Mars Science Center." << endl;
	cout << "\tR: Toggle display of the roads." << endl;
	cout << "\tL: Toggle display of the labels." << endl;
	cout << endl;
	cout << "Mouse Commands:" << endl;
	cout << "\tClicking on a building will display a larger version of the corresponding label." << endl;
	cout << "\tClick'n'drag to pan." << endl;
	cout << "\tMouse wheel to zoom in/out." << endl;
}

/*
Main function for the program.
Precondition: None.
Postconditino: Everything has been initialized, and the GLUT main loop has been started.
*/
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

	// Display user controls.
	introInstructions();

	// Begin processing.
	glutMainLoop();

	return 0;
}