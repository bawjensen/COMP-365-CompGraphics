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

// -------------------------------------------------------------------------------------------

// string DEMFileName = "test.dem.grd";
// string DEMFileName = "crater.dem.grd";
string DEMFileName = "";

int initialWindowWidth = 800;
int initialWindowHeight = 800;

int currentWindowWidth = initialWindowWidth;
int currentWindowHeight = initialWindowHeight;

float verticalFieldOfView = 60.0f;
float horizFieldOfView = ((float)initialWindowWidth / (float)initialWindowHeight) * verticalFieldOfView;

// grid dimensions for the reference grid (used in debugging)
int gridWidth = 1200;
int gridBoundary = gridWidth / 2;

// radius of rotation for the camera
int rotationRadius = 1250;

bool gettingRandomDEMFileInput = false;

// Camera mCam;
User user;
Ground ground;
DEMGenerator gridGen;

// -------------------------------------------------------------------------------------------

void quit() {
	exit(1);
}

// -------------------------------------------------------------------------------------------

void initGenMenu(); // Forward delclaration to allow use in menuCallback
void generateAndSetGround(); // Forward delclaration to allow use in menuCallback

void userInputAndInstructions() {
	char choice;
	cout << "Use pre-built ESRI DEM file (P) or randomly generate ESRI DEM file (R): ";
	cin >> choice;

	if (choice == 'P') {
		cout << "Enter filename: ";
		cin >> DEMFileName;
	}
	else if (choice == 'R') {
		gettingRandomDEMFileInput = true;

		cout << endl;
		cout << "Enter values for: " << endl;

		cout << "Name of output DEM grid file: ";
		// cin >> gridGen.outFileName;
		gridGen.outFileName = "temp.dem.grd";

		cout << "Grid width (grid is square - preferred: 2^n + 1): ";
		// cin >> gridGen.gridWidth;
		gridGen.gridWidth = 65;

		cout << "Grid spacing (a.k.a. cell size): ";
		// cin >> gridGen.cellSize;
		gridGen.cellSize = 1;

		cout << "Fractal generation's roughness factor: ";
		// cin >> gridGen.roughnessFactor;
		gridGen.roughnessFactor = 2.5;

		cout << "Number of smoothing iterations (creates a more even surface): ";
		// cin >> gridGen.numSmooths;
		gridGen.numSmooths = 2;

		cout << "Defined points incrementation value (size by which user input points go up): ";
		// cin >> gridGen.incrAmount;
		gridGen.incrAmount = 1.0f;

		gridGen.initialize();

		cout << endl;
	}

	// TODO: Print intsructions
	// r = lake
	// R = big lake
	// scroll wheel = elevations
	// menu for creation
	// WASD movement - space up, x down
	// etc
}

void updateMovementSpeed() {
	user.moveSpeed = max(ground.nRows, ground.nCols) / 100.0f;
}

void menuCallback(int choice) {
	switch(choice) {
		case -1: 	quit();
					break;
		case 1: 	ground.readFromESRIFile(gridGen.outFileName);
					break;
		case 2: 	gridGen.initialize();
					gettingRandomDEMFileInput = true;
					initGenMenu();
					// ground.readFromESRIFile(gridGen.createGridFile());
					break;
		case 3: 	ground.readFromESRIFile("grds/coolLake.dem.grd");
					break;
		case 4: 	ground.readFromESRIFile("grds/hillside.dem.grd");
					break;
		case 5: 	ground.readFromESRIFile("grds/smoothMount.dem.grd");
					break;
		case 10: 	generateAndSetGround();
					break;
	}

	updateMovementSpeed();
}

void initMenu() {
	glutCreateMenu(menuCallback);

	glutAddMenuEntry("Choices:", 0);
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Switch DEM file:", 0);
	glutAddMenuEntry("\tGenerated Scene", 1);
	glutAddMenuEntry("\tGenerate New Random Scene", 2);
	glutAddMenuEntry("\tLake Scene", 3);
	glutAddMenuEntry("\tHill Side Scene", 4);
	glutAddMenuEntry("\tSmooth Mountain Scene", 5);
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Quit", -1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void generateAndSetGround() {
	gettingRandomDEMFileInput = false; // Done getting input - creating ground now

	DEMFileName = gridGen.createGridFile();

	ground.readFromESRIFile(DEMFileName);

	initMenu();
}

void initGenMenu() {
	glutCreateMenu(menuCallback);

	glutAddMenuEntry("Choices:", 0);
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Create Ground", 10);
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Quit", -1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init(int numArgs, char** argArray) {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);

	// Turn on OpenGL lighting.
	glEnable(GL_LIGHTING);

	// Light property vectors.
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

	// Light0 properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);

	glEnable(GL_LIGHT0); // Enable particular light source.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint

	user.setDepthOfView(1000);

	if (!gettingRandomDEMFileInput){
		ground.readFromESRIFile(DEMFileName);
		initMenu();
	}
	else {
		gridGen.eyePointer = &user;
		initGenMenu();
	}

	// cout << "\n\nTesting:" << endl;

	// Vec3f vec(1, 0, 0);
	// Vec3f axis(0, 0, -1);

	// Quat4f quat(45, axis);

	// cout << quat.rotateVector(vec) << endl;

	// cout << "Testing done.\n\n" << endl;
}

void resize(int w, int h) {
	if (h == 0) h = 1; // Prevent division by 0

	currentWindowWidth = w;
	currentWindowHeight = h;

	float ratio =  (float)w / h;

	horizFieldOfView = ratio * verticalFieldOfView;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(verticalFieldOfView, ratio, 0.1f, user.depthOfView);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// -------------------------------------------------------------------------------------------

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
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(10);
	glBegin(GL_LINES);
		glVertex3f(-gridBoundary * 1.1, 0, 	0);
		glVertex3f(gridBoundary * 1.1, 	0, 	0);
		glVertex3f(0, 	0, 	-gridBoundary * 1.1);
		glVertex3f(0, 	0, 	gridBoundary * 1.1);
	glEnd();

	// Draw grid on "ground"
	glColor3f(0.9, 0.9, 0.9);
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

	// Light position vectors.
	float lightPos[] = { 200.0, 1000.0, 0.0, 1.0 };

	float amb = 1.0;
	// float diff = 0.5;
	float spec = 0.0;

	// Material property vectors.
	float matAmb[] = { amb, amb, amb, 1.0 };
	// float matDif[] = { 0.0, diff, 0.0, 1.0 };
	float matSpec[] = { spec, spec, spec, 1.0 };
	float matShine[] = { 50.0 };
	float matEmission[] = { 0.0, 0.0, 0.0, 1.0 };

	// Light quadratic attenuation factor.
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

	user.update();

	// Set the camera
	gluLookAt(	user.pos.x,					user.pos.y,	 				user.pos.z,
				user.pos.x+user.viewDir.x, 	user.pos.y+user.viewDir.y, 	user.pos.z+user.viewDir.z,
				// user.focus.x, 				user.focus.y, 				user.focus.z,
				0.0f, 						1.0f,  						0.0f);

	// Draw light source spheres after disabling lighting.
	// glDisable(GL_LIGHTING);

	// Light0 and its sphere positioned.
	// glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	// glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
	// glColor3f(1.0, 1.0, 1.0);
	// glScalef(3.0, 3.0, 3.0);
	// glutWireSphere(0.05, 8, 8);
	// glPopMatrix();

	// glEnable(GL_LIGHTING);

	// Material properties of sphere.
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	// glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);


	// glDisable(GL_LIGHTING);
	// drawReferenceGrid();
	// drawAxes();
	// glEnable(GL_LIGHTING);

	if (gettingRandomDEMFileInput)
		gridGen.display();
	else
		ground.display();

	// Swap the buffers - flushing the current buffer
	glutSwapBuffers();
}

void idle() {
	display();
}

// -------------------------------------------------------------------------------------------

void mousePassiveMoveCallback(int x, int y) {
}

void mouseMoveCallback(int x, int y) {
	user.handleMovement(x, y);
}

void mouseCallback(int button, int state, int x, int y) {
	user.handleClick(button, state, x, y);

	// float horizAngle = ((float)x / currentWindowWidth - 0.5) * horizFieldOfView;
	// float vertAngle = ((float)y / currentWindowHeight - 0.5) * verticalFieldOfView;
	gridGen.handleClick(button, state, x, y);
}

void keyUpCallback(unsigned char key, int x, int y) {
	switch(key) {
		case 27:	quit();
					break;
		case ' ':	user.stopMove(Camera::UP);
					break;
		case 'x':	user.stopMove(Camera::DOWN);
					break;
		case 's':
		case '2':	user.stopMove(Camera::BACKWARD);
					break;
		case 'w':
		case '8':	user.stopMove(Camera::FORWARD);
					break;
		case 'a':
		case '4':	user.stopMove(Camera::LEFT);
					break;
		case 'd':
		case '6':	user.stopMove(Camera::RIGHT);
					break;
	}
}

void keyDownCallback(unsigned char key, int x, int y) {
	switch(key) {
		case 27:	quit();
					break;
		case ' ':	user.move(Camera::UP);
					break;
		case 'x':	user.move(Camera::DOWN);
					break;
		case 's':
		case '2':	user.move(Camera::BACKWARD);
					break;
		case 'w':
		case '8':	user.move(Camera::FORWARD);
					break;
		case 'a':
		case '4':	user.move(Camera::LEFT);
					break;
		case 'd':
		case '6':	user.move(Camera::RIGHT);
					break;



		case 'q': 	mouseCallback(3, GLUT_DOWN, x, y);
					break;
		case 'Q': 	mouseCallback(4, GLUT_DOWN, x, y);
					break;

		case 'r': 	gridGen.addLake(x, y, 2);
					break;
		case 'R': 	gridGen.addLake(x, y, 3);
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
	switch (key) {
		case GLUT_KEY_UP: keyUpCallback('8', x, y);
					break;
		case GLUT_KEY_DOWN: keyUpCallback('2', x, y);
					break;
		case GLUT_KEY_LEFT: keyUpCallback('4', x, y);
					break;
		case GLUT_KEY_RIGHT: keyUpCallback('6', x, y);
					break;
	}
}

// -------------------------------------------------------------------------------------------

int main(int argc, char** argv) {
	userInputAndInstructions();

	// Init GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// Create GLUT window
	glutInitWindowSize(initialWindowWidth, initialWindowHeight); // Scale factor is scaling the map to fit screen 
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("Lit Landscape");

	// Custom init
	init(argc, argv);

	// Register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMouseFunc(mouseCallback); // Mouse up/down
	glutMotionFunc(mouseMoveCallback); // Mouse movement
	glutPassiveMotionFunc(mousePassiveMoveCallback); // Mouse movement

	// glutIgnoreKeyRepeat(1); // Ignore the repetition of chars that occur when holding a key
	glutKeyboardFunc(keyDownCallback); // Key down
	glutKeyboardUpFunc(keyUpCallback); // Key up
	glutSpecialFunc(specialDownCallback); // Special key down
	glutSpecialUpFunc(specialUpCallback); // Special key up

	// Begin processing.
	glutMainLoop();

	return 0;
}