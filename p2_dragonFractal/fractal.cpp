/*	Name: Bryan Jensen
	Date: 2/19/14
	Filename: fractal.cpp

	Description: This program provides a graphical user interface for displaying a "Dragon Curve"
				 fractal, along with various configuration choices. To achieve this, OpenGL along
				 with GLUT are used to create a graphics window wherein the program executes and
				 displays.

	Input: All input is achieved through user clicks inside the graphics window. Various inputs
		   have differing effects. Quitting the program is achieved with the letter q or the
		   escape key.

	Output:	The program will display any and all output as graphical output in the GLUT window
			context, as well as secondary feedback messages into the stdio output (aka the terminal
			window).
*/

#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif

#include <iostream>

#include "classes.h"

using namespace std;

static int initialWindowWidth = 500; // Starting window size - useful for resizing ratios
static int initialWindowHeight = 500; // Starting window size - useful for resizing ratios
static int userWindowWidth = initialWindowWidth; // Variable for current graphical window size
static int userWindowHeight = initialWindowHeight; // Variable for current graphical window size
static float windowRatio = 2; // Ratio of initial window to initial ortho dimensions
static int windowWidth = initialWindowWidth * windowRatio; // Refers to the OpenGL Ortho window
static int windowHeight = initialWindowHeight * windowRatio; // Refers to the OpenGL Ortho window
static int widthOffset = 0; // Initial offset for ortho dimensions
static int heightOffset = 0; // Initial offset for ortho dimensions

static Fractal dFractal; // The fractal to be created and drawn

static const int KEY_ESCAPE = 27; // The integer value for the keyboard key "esc"

// Callback for keyboard key press
void keyDownCallback(unsigned char key, int x, int y) { // Note: x and y are from cursor
	switch(key) {
		case KEY_ESCAPE:
		case 'q': 	exit(1);
	}
}

// Callback for mouse click
void mouseCallback(int button, int state, int x, int y) {
	if (state == GLUT_UP or button == GLUT_RIGHT_BUTTON) return;

	// Normalize the click coordinates to the ortho coordinates
	y = userWindowHeight - y;

	x *= windowRatio;
	y *= windowRatio;

	x += widthOffset;
	y += heightOffset;

	// Pass the coordinates into the fractal with the desired behavior
	if (dFractal.begun()) dFractal.incrementIterations();
	else dFractal.begin(x, y);
}

// Callback for graphical window resizing
void resize(int width, int height) {
	// Update globals
	userWindowWidth = width;
	userWindowHeight = height;

	widthOffset = initialWindowWidth - width;
	heightOffset = initialWindowHeight - height;

	// Update viewport to full size
	glViewport(0, 0, width, height);

	// Calculate the offsets and such
	int left = initialWindowWidth - width;
	int right = left + (width * windowRatio);

	int bottom = initialWindowHeight - height;
	int top = bottom + (height * windowRatio);

	// Reset the ortho
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);

	// And finally redisplay
	glutPostRedisplay();
}

// Callback for a menu event (triggered by the menu-bound click event)
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

// Initialization function for menu and options
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

// General initialization for OpenGL
void init() {
	glClearColor(0.875, 0.875, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	initMenu();

	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);

	cout << "Program initialized. NOTE: q or esc will quit the program." << endl;
}

// Display loop function (called by GLUT)
void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	dFractal.draw();

	glFlush();
}

// Main function
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