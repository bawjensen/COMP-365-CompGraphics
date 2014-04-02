#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif

#include <cstdlib>
#include <cmath>
#include <iostream>

#include "classes.h"

using namespace std;

// string DEMFileName = "small.dem.grd";
// string DEMFileName = "test.dem.grd";
string DEMFileName = "test2.dem.grd";
// string DEMFileName = "mt257.dem.grd";
// string DEMFileName = "tucks.dem.grd";

string grammarFileName = "test.gram";

int initialWindowWidth = 800;
int initialWindowHeight = 800;

int gridWidth = 1200;
int gridBoundary = gridWidth / 2;

// radius of rotation for the camera
int rotationRadius = 1250;

Camera mCam;
Ground ground;
PlantLandscape pLand;
Minimap minimap(200, 200, initialWindowWidth, initialWindowHeight, &ground, &pLand);

void quit() {
	exit(1);
}

void printUserInstructions() {
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

void drawReferenceGrid() {
	// Draw axes
	glLineWidth(4);
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
	switch(choice) {
		case 0: quit();
				break;
	}
}

void initMenu() {
	glutCreateMenu(menuCallback);

	glutAddMenuEntry("Choices:", -1);
	glutAddMenuEntry("", -1);
	glutAddMenuEntry("Mode: Linear Splines", 1);
	glutAddMenuEntry("Mode: Quadratic Splines", 2);
	glutAddMenuEntry("Mode: No Splines", 3);
	glutAddMenuEntry("Increase Grid Elevation", 4);
	glutAddMenuEntry("Decrease Grid Elevation", 5);
	glutAddMenuEntry("Reset Grid", 6);
	glutAddMenuEntry("Quit", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init() {
	glEnable(GL_DEPTH_TEST);

	ground.readFromESRIFile(DEMFileName);
	pLand.loadGrammar(grammarFileName);

	mCam.setDepthOfView(ground.cellSize * max(ground.nCols, ground.nRows) * 2.5);

	initMenu();

	printUserInstructions();

	mCam.pos = Vec3f(0, 55, -75);

	// for (int i = 0; i < 1; i++) {
	// 	cout << pLand.generatePlantString(0) << endl;
	// }
}

int main(int argc, char** argv) {
	// Init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(initialWindowWidth, initialWindowHeight); // Scale factor is scaling the map to fit screen 
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("Terrain Splines");

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
	init();

	// Begin processing.
	glutMainLoop();

	return 0;
}