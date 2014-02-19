#include <GL/glut.h>
#include <iostream>

#include "classes.h"

using namespace std;

static int initialWindowWidth = 500;
static int initialWindowHeight = 500;
static int userWindowWidth = initialWindowWidth;
static int userWindowHeight = initialWindowHeight;
static float windowRatio = 2;
static int windowWidth = initialWindowWidth * windowRatio; // Refers to the GL window
static int windowHeight = initialWindowHeight * windowRatio; // Refers to the GL window
static int widthOffset = 0;
static int heightOffset = 0;

static Fractal dFractal;

static const int KEY_ESCAPE = 27;

void keyDownCallback(unsigned char key, int x, int y) { // Note: x and y are from cursor
	switch(key) {
		case KEY_ESCAPE:
		case 'q': 	exit(1);
	}
}

void mouseCallback(int button, int state, int x, int y) {
	if (state == GLUT_UP or button == GLUT_RIGHT_BUTTON) return;

	y = userWindowHeight - y;

	x *= windowRatio;
	y *= windowRatio;

	x += widthOffset;
	y += heightOffset;

	if (dFractal.begun()) dFractal.incrementIterations();
	else dFractal.begin(x, y);
}

void resize(int width, int height) {
	userWindowWidth = width;
	userWindowHeight = height;

	widthOffset = initialWindowWidth - width;
	heightOffset = initialWindowHeight - height;

	glViewport(0, 0, width, height);

	int left = initialWindowWidth - width;
	int right = left + (width * windowRatio);

	int bottom = initialWindowHeight - height;
	int top = bottom + (height * windowRatio);

	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);

	glutPostRedisplay();
}

void menuCallback(int choice) {
	switch (choice) {
		case -9:keyDownCallback('q', 0, 0); // Redirect call - mimic hitting 'q'
		case 0:	dFractal.hide();
				break;
		case 1:	dFractal.show();
				break;
		case 2:	dFractal.reset();
				break;
		case 3:	dFractal.setInitialLength(1);
				break;
		case 4:	dFractal.setInitialLength(2);
				break;
		case 5:	dFractal.setDirection("c");
				break;
		case 6:	dFractal.setDirection("cc");
				break;
		case 7:	dFractal.setColor("red");
				break;
		case 8:	dFractal.setColor("green");
				break;
		case 9:	dFractal.setColor("blue");
				break;
		case 10:dFractal.alternate(20);
				break;
	}
}

void initMenu() {
	glutCreateMenu(menuCallback);

	glutAddMenuEntry("Choices:", -1);
	glutAddMenuEntry("", -1);
	glutAddMenuEntry("Clear the window", 0);
	glutAddMenuEntry("Redisplay the fractal", 1);
	glutAddMenuEntry("Clear the fractal", 2);
	glutAddMenuEntry("Starting length: 1", 3);
	glutAddMenuEntry("Starting length: 2", 4);
	glutAddMenuEntry("Set spiral direction: Clockwise", 5);
	glutAddMenuEntry("Set spiral direction: Counter-Clockwise", 6);
	glutAddMenuEntry("Set Color: Red", 7);
	glutAddMenuEntry("Set Color: Green", 8);
	glutAddMenuEntry("Set Color: Blue", 9);
	glutAddMenuEntry("Create alternation pattern", 10);
	glutAddMenuEntry("Quit", -9);

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

	// glBegin(GL_POLYGON);
	// 	glColor3f(1.0, 0.0, 0.0);
	// 	glVertex2i(0, 0);
	// 	glVertex2i(0, 1000);
	// 	glVertex2i(1000, 1000);
	// 	glVertex2i(1000, 0);
	// glEnd();

	dFractal.draw();

	glFlush();
}

int main(int argc, char** argv) {
	// Initialize GLUT.
	glutInit(&argc, argv);
	// Set display mode as single-buffered and RGB color.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	// Set OpenGL window size.
	glutInitWindowSize(initialWindowWidth, initialWindowHeight); // Scale factor is scaling the map to fit screen 
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