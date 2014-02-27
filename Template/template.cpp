#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif

#include <iostream>

using namespace std;

static int windowWidth = 1000;
static int windowHeight = 1000;


void keyDownCallback(unsigned char key, int x, int y) { // Note: x and y are from cursor
}

void mouseCallback(int button, int state, int x, int y) {
}

void resize(int width, int height) {
	glutPostRedisplay();
}

void menuCallback(int choice) {
}

void initMenu() {
	glutCreateMenu(menuCallback);

	glutAddMenuEntry("Choices:", -1);
	glutAddMenuEntry("", -1);

	glutAttachMenu (GLUT_RIGHT_BUTTON);
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	initMenu();

	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

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