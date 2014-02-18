#include <GL/glut.h>
#include <iostream>

#include "classes.h"

using namespace std;

static int userWindowWidth = 500;
static int userWindowHeight = 500;
static int windowWidth = 1000; // Refers to the GL window
static int windowHeight = 1000; // Refers to the GL window

static Fractal dFractal;

static const int KEY_ESCAPE = 27;

void keyDownCallback(unsigned char key, int x, int y) { // Note: x and y are from cursor
	switch(key) {
		case KEY_ESCAPE:
		case 'q': 	exit(1);
	}
}

void mouseCallback(int button, int state, int x, int y) {
	if (state == GLUT_UP) return;

	if (dFractal.begun()) dFractal.incrementIterations();
	else dFractal.begin(x, y);
}

void resize(int w, int h) {
	userWindowWidth = w;
	userWindowHeight = h;

	glutPostRedisplay();
}

void menuCallback(int choice) {
	switch (choice) {
		case 0:	dFractal.hide();
				break;
		case 1:	dFractal.show();
				break;
		case 2:	dFractal.clear();
				break;
		case 3:	dFractal.setInitialLength(1);
				break;
		case 4:	dFractal.setInitialLength(2);
				break;
	}
}

void initMenu() {
	glutCreateMenu(menuCallback);

	glutAddMenuEntry ("Choices:", -1);
	glutAddMenuEntry ("", -1);
	glutAddMenuEntry ("Clear the window", 0);
	glutAddMenuEntry ("Redisplay the fractal", 1);
	glutAddMenuEntry ("Clear the fractal", 2);
	glutAddMenuEntry ("Starting length: 1", 3);
	glutAddMenuEntry ("Starting length: 2", 4);

	glutAttachMenu (GLUT_RIGHT_BUTTON);
}

void init() {
	glClearColor(0.875, 0.875, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	initMenu();

	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	dFractal.draw();

	glFlush();
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
	glutCreateWindow("Dragon Curve Fractal");

	// Initialize.
	init();

	// Register key-down routine.
	glutKeyboardFunc(keyDownCallback);
	// Register mouse routine.
	glutMouseFunc(mouseCallback);
	// Register display routine.
	glutDisplayFunc(display);
	// Register reshape routine.
	glutReshapeFunc(resize);

	// Begin processing.
	glutMainLoop();

	return 0;
}