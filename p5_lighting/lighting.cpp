#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "classes.h"
#include "baseClasses.h"

using namespace std;

string DEMFileName = "small.dem.grd";
// string DEMFileName = "test.dem.grd";
// string DEMFileName = "test2.dem.grd";
// string DEMFileName = "mt257.dem.grd";
// string DEMFileName = "tucks.dem.grd";

int initialWindowWidth = 800;
int initialWindowHeight = 800;

// grid dimensions for the reference grid (used in debugging)
int gridWidth = 1200;
int gridBoundary = gridWidth / 2;

// radius of rotation for the camera
int rotationRadius = 1250;

Camera mCam;

void quit() {
	exit(1);
}

string toString(int x) {
	ostringstream convert;

	convert << x;

	return convert.str();
}

void printUserInstructions() {
}

void getUserInputStdDev() {
	cout << "Standard deviation (current value : " << pLand.stdDev << ") to use in randomness factor of plant angles: ";
	cin >> pLand.stdDev;

	cout << "Standard deviation of " << pLand.stdDev << " set!" << endl;
}

void resize(int w, int h) {
	if (h == 0) h = 1; // Prevent division by 0

	float ratio =  (float)w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(60.0f, ratio, 0.1f, mCam.depthOfView);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void drawAxes() {
	// Draw axes
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(-1000, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, -1000, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, -1000);

	glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1000, 0, 0);
	glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1000, 0);
	glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1000);
	glEnd();
}

void drawReferenceGrid() {
	// Draw axes
	glLineWidth(10);
	glBegin(GL_LINES);
		glVertex3f(-gridBoundary * 1.1, 0, 	0);
		glVertex3f(gridBoundary * 1.1, 	0, 	0);
		glVertex3f(0, 	0, 	-gridBoundary * 1.1);
		glVertex3f(0, 	0, 	gridBoundary * 1.1);
	glEnd();

	// Draw grid on "ground"
	glColor3f(0.9f, 0.9f, 0.9f);
	glLineWidth(1);
	glBegin(GL_LINES);
	int gridIterator = gridBoundary / 10;
	for (int i = -gridBoundary; i <= gridBoundary; i += gridIterator) {
		glVertex3f(i, 0, -gridBoundary);
		glVertex3f(i, 0, gridBoundary);
	}
	for (int j = -gridBoundary; j <= gridBoundary; j += gridIterator) {
		glVertex3f(-gridBoundary, 0, j);
		glVertex3f(gridBoundary, 0, j);
	}
	glEnd();
}

void display() {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	mCam.update();

	// Set the camera
	gluLookAt(	mCam.pos.x,					mCam.pos.y,	 				mCam.pos.z,
				mCam.pos.x+mCam.viewDir.x, 	mCam.pos.y+mCam.viewDir.y, 	mCam.pos.z+mCam.viewDir.z,
				// mCam.focus.x, 				mCam.focus.y, 				mCam.focus.z,
				0.0f, 						1.0f,  						0.0f);

	// drawReferenceGrid();
	// drawAxes();

	ground.display();
	minimap.displayIndicator();
	pLand.display();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, initialWindowWidth, 0, initialWindowHeight, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	minimap.display();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// Swap the buffers - flushing the current buffer
	glutSwapBuffers();
}

void idle() {
	display();
}

void keyUpCallback(unsigned char key, int x, int y) {
	switch(key) {
		case 27:	quit();
					break;
		case ' ':	mCam.stopMove(Camera::UP);
					break;
		case 'x':	mCam.stopMove(Camera::DOWN);
					break;
		case 's':
		case '2':	mCam.stopMove(Camera::BACKWARD);
					break;
		case 'w':
		case '8':	mCam.stopMove(Camera::FORWARD);
					break;
		case 'a':
		case '4':	mCam.stopMove(Camera::LEFT);
					break;
		case 'd':
		case '6':	mCam.stopMove(Camera::RIGHT);
					break;
	}
}

void keyDownCallback(unsigned char key, int x, int y) {
	switch(key) {
		case 27:	quit();
					break;
		case ' ':	mCam.move(Camera::UP);
					break;
		case 'x':	mCam.move(Camera::DOWN);
					break;
		case 's':
		case '2':	mCam.move(Camera::BACKWARD);
					break;
		case 'w':
		case '8':	mCam.move(Camera::FORWARD);
					break;
		case 'a':
		case '4':	mCam.move(Camera::LEFT);
					break;
		case 'd':
		case '6':	mCam.move(Camera::RIGHT);
					break;
	}
}

void specialDownCallback(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP: keyDownCallback('8', x, y);
					break;
		case GLUT_KEY_DOWN: keyDownCallback('2', x, y);
					break;
		case GLUT_KEY_LEFT: keyDownCallback('4', x, y);
					break;
		case GLUT_KEY_RIGHT: keyDownCallback('6', x, y);
					break;
	}
}

void specialUpCallback(int key, int x, int y) { 	
}

void mousePassiveMoveCallback(int x, int y) {
	minimap.handleMovement(x, y);
}

void mouseMoveCallback(int x, int y) {
	mCam.handleMovement(x, y);
}

void mouseCallback(int button, int state, int x, int y) {
	mCam.handleClick(button, state, x, y);

	// pLand.handleClick(button, state, x, y);
	minimap.handleClick(button, state, x, y);
}

void menuCallback(int choice) {
	if (choice >= 100) { // Choosing a custom grammar
		pLand.currPlantType = (choice - 100 + 3); // Choice shifted over 100, and add 3 for the 3 built in
		return;
	}

	switch(choice) {
		case -1: 	quit();
					break;
		case 1: 	pLand.currPlantType = 0;
					break;
		case 2: 	pLand.currPlantType = 1;
					break;
		case 3: 	pLand.currPlantType = 2;
					break;
		case 4: 	pLand.defaultScene();
					break;
		case 5: 	pLand.currentRandomPlace(3);
					break;
		case 6: 	pLand.randomRandomPlace(5);
					break;
		case 7: 	pLand.clear();
					break;
		case 8:		getUserInputStdDev();
	}
}

void initMenu() {
	glutCreateMenu(menuCallback);

	glutAddMenuEntry("Choices:", 0);
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Show Default Scene", 4);
	glutAddMenuEntry("Randomly Place 3 Of The Current Plant", 5);
	glutAddMenuEntry("Randomly Place 5 Random Plants", 6);
	glutAddMenuEntry("Clear All Plants", 7);
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Choose: Grammar (Tree)", 1); // 1
	glutAddMenuEntry("Choose: Grammar (Bush)", 2); // 2
	glutAddMenuEntry("Choose: Grammar (Flower Twig)", 3); // 3

	int n = 100; // The starting number for the custom grammars
	string entryLabel;
	for (int i = 0; i < pLand.nGrammars - 3; i++) {
		entryLabel = "Choose: Custom Grammar " + toString(i+1);
		glutAddMenuEntry(entryLabel.c_str(), n + i);
	}

	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Input standard deviation (via the console)", 8);
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Quit", -1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init(int numArgs, char** argArray) {
	glEnable(GL_DEPTH_TEST);

	pLand.groundPointer = &ground;

	ground.readFromESRIFile(DEMFileName);

	pLand.loadGrammar(grammarFileName1);
	pLand.loadGrammar(grammarFileName2);
	pLand.loadGrammar(grammarFileName3);

	for (int i = 1; i < numArgs; i++) {
		pLand.loadGrammar(argArray[i]);
	}

	mCam.setDepthOfView(ground.cellSize * max(ground.nCols, ground.nRows) * 2.5);

	initMenu();

	printUserInstructions();
}

int main(int argc, char** argv) {
	// Init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(initialWindowWidth, initialWindowHeight); // Scale factor is scaling the map to fit screen 
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("Fractal Landscape");

	// Register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMouseFunc(mouseCallback); // Mouse up/down
	glutMotionFunc(mouseMoveCallback); // Mouse movement
	glutPassiveMotionFunc(mousePassiveMoveCallback); // Mouse movement

	glutIgnoreKeyRepeat(1); // Ignore the repetition of chars that occur when holding a key
	glutKeyboardFunc(keyDownCallback); // Key down
	glutKeyboardUpFunc(keyUpCallback); // Key up
	glutSpecialFunc(specialDownCallback); // Special key down
	glutSpecialUpFunc(specialUpCallback); // Special key up

	// Custom init
	init(argc, argv);

	// Begin processing.
	glutMainLoop();

	return 0;
}