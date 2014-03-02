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

static int windowWidth = 1200;
static int windowHeight = 1200;
static int windowDepth = 1000;
static int initialWindowWidth = 1000;
static int initialWindowHeight = 1000;
static int userWindowWidth = initialWindowWidth;
static int userWindowHeight = initialWindowHeight;

// baseAngle of rotation for the camera direction
float baseAngle = 0.0f;
float deltaAngle = 0.0f;
float angleScrollSpeed = 1 / (float)userWindowWidth;

Vec3f camView(0.0f, 0.0f, -1.0f);
Vec3f camPos(0.0f, 10.0f, 0.0f);

int xOrigin = 0; // Equals 0 when not using mouse to pan
int yOrigin = 0; // Equals 0 when not using mouse to pan

void resize(int w, int h) {
	if (h == 0) h = 1; // Prevent division by 0

	float ratio =  (float)w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(60.0f, ratio, 0.1f, 1000.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void display() {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	camPos.x,			camPos.y,	 		camPos.z,
				camPos.x+camView.x, camPos.y+camView.y, camPos.z+camView.z,
				0.0f, 				1.0f,  				0.0f);

	// Draw grid on ground
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_LINES);
	for (int i = -100; i <= 100; i += 10) {
			glVertex3f(i, 0, -100);
			glVertex3f(i, 0, 100);
	}
	for (int j = -100; j <= 100; j += 10) {
			glVertex3f(-100, 0, j);
			glVertex3f(100, 0, j);
	}
	glEnd();

	glutSwapBuffers();
}

void idle() {
	display();
}

void keyDownCallback(unsigned char key, int xx, int yy) { 	
	if (key == 27)
		exit(0);
} 

void specialDownCallback(int key, int xx, int yy) {
	switch (key) {
		case GLUT_KEY_UP: break;
		case GLUT_KEY_DOWN: break;
	}
} 

void specialUpCallback(int key, int x, int y) { 	
	switch (key) {
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN: break;
	}
} 

void mouseMoveCallback(int x, int y) { 	
	// this will only be true when the left button is down
	if (xOrigin != 0) {

		// update deltaAngle
		deltaAngle = (xOrigin - x) * angleScrollSpeed;

		// update camera's direction
		camView.x = sin(baseAngle + deltaAngle);
		camView.z = -cos(baseAngle + deltaAngle);
	}
}

void mouseCallback(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) { // only start motion if the left button is pressed
		if (state == GLUT_UP) { // when the button is released
			baseAngle += deltaAngle;
			xOrigin = 0;
			yOrigin = 0;
		}
		else  { // state = GLUT_DOWN
			xOrigin = x;
			yOrigin = y;
		}
	}
}

void init() {
	glEnable(GL_DEPTH_TEST);
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

	glutIgnoreKeyRepeat(1); // Ignore the repetition of chars that occur when holding a key
	glutKeyboardFunc(keyDownCallback); // Key down
	glutSpecialFunc(specialDownCallback); // Special key down
	glutSpecialUpFunc(specialUpCallback); // Special key up

	// Custom init
	init();

	// Begin processing.
	glutMainLoop();

	return 0;
}