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

// string DEMFileName = "test.dem.grd";
// string DEMFileName = "test2.dem.grd";
// string DEMFileName = "mt257.dem.grd";
string DEMFileName = "tucks.dem.grd";

int initialWindowWidth = 1000;
int initialWindowHeight = 1000;

int gridWidth = 1200;
int gridBoundary = gridWidth / 2;

// radius of rotation for the camera
int rotationRadius = 1250;

Camera mCam;
SplineGrid spGrid;

void quit() {
	exit(1);
}

void printUserInstructions() {
	cout << "Keyboard commands:" << endl;
	cout << "\t1 - Zoom In" << endl;
	cout << "\t7 - Zoom Out" << endl;
	cout << "\t8 - Rotate Up" << endl;
	cout << "\t4 - Rotate Left" << endl;
	cout << "\t6 - Rotate Right" << endl;
	cout << "\t2 - Rotate Down" << endl;
	cout << "\tq / esc - Quit" << endl;

	cout << "You may also use mouse to pan/rotate, and right click will activate a menu with more options." << endl;

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

	// Set the camera
	gluLookAt(	mCam.pos.x,			mCam.pos.y,	 		mCam.pos.z,
				// camPos.x+camView.x, camPos.y+camView.y, camPos.z+camView.z,
				mCam.focus.x, 		mCam.focus.y, 		mCam.focus.z,
				0.0f, 				1.0f,  				0.0f);

	// drawReferenceGrid();

	spGrid.display();

	// Swap the buffers - flushing the current buffer
	glutSwapBuffers();
}

void idle() {
	display();
}

void keyDownCallback(unsigned char key, int x, int y) {
	switch(key) {
		case 27:
		case 'q':	quit();
		case '1':	mCam.setRotationRadius(mCam.rotationRadius * 0.9);
					break;
		case '7':	mCam.setRotationRadius(mCam.rotationRadius / 0.9);
					break;
		case '2':	mCam.rotate(0, M_PI / 16);
					break;
		case '8':	mCam.rotate(0, -M_PI / 16);
					break;
		case '4':	mCam.rotate(M_PI / 16, 0);
					break;
		case '6':	mCam.rotate(-M_PI / 16, 0);
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

void mouseMoveCallback(int x, int y) {
	mCam.handleMovement(x, y);
}

void mouseCallback(int button, int state, int x, int y) {
	mCam.handleClick(button, state, x, y);
}

void menuCallback(int choice) {
	switch(choice) {
		case 0: quit();
				break;
		case 1:	spGrid.setMode(SplineGrid::MODE_LINEAR);
				break;
		case 2:	spGrid.setMode(SplineGrid::MODE_QUADRATIC);
				break;
		case 3:	spGrid.setMode(SplineGrid::MODE_KNOTS);
				break;
		case 4:	spGrid.changeGridElevation(2);
				break;
		case 5:	spGrid.changeGridElevation(.5);
				break;
		case 6:	spGrid.setMode(SplineGrid::MODE_KNOTS);
				spGrid.changeGridElevation(0);
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
	// glEnable(GL_DEPTH_TEST);

	mCam.setPos(rotationRadius, 0, 0);

	spGrid.readFromESRIFile(DEMFileName);

	mCam.setRotationRadius(spGrid.greaterDimension);
	mCam.setFocus(0, (float)(spGrid.highest + spGrid.lowest) / 2, 0);

	initMenu();

	printUserInstructions();
}

int main(int argc, char** argv) {
	// Init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(/*GLUT_DEPTH |*/ GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(initialWindowWidth, initialWindowHeight); // Scale factor is scaling the map to fit screen 
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("Terrain Splines");

	// Register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMouseFunc(mouseCallback); // Mouse up/down
	glutMotionFunc(mouseMoveCallback); // Mouse movement

	// glutIgnoreKeyRepeat(1); // Ignore the repetition of chars that occur when holding a key
	glutKeyboardFunc(keyDownCallback); // Key down
	glutSpecialFunc(specialDownCallback); // Special key down
	glutSpecialUpFunc(specialUpCallback); // Special key up

	// Custom init
	init();

	// Begin processing.
	glutMainLoop();

	return 0;
}