/* moveTeapot.cpp
 *
 * Starting code for in-class exercise to animate the teapot.
 *
 */

#include <iostream>
#include <GL/glut.h>

using namespace std;

float z = 5.0;  // distance of eye (center of projection) from frustum
const int NUM_TICKS = 15000;
const float ROTATION_INCR = 3.0;
float rotation = 0.0;

void display ()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glLoadIdentity ();
	glPushMatrix();
	gluLookAt (0.0, 0.0, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glScalef (1.0, 1.0, 1.0);

	/* draw a red cube in center of volume */
	glColor3f (1.0, 0.0, 0.0);
	glutWireCube (0.8);


	/* draw the teapot */
	glColor3f (0.2, 0.2, 1.0);
	glRotatef(rotation, 0.0, 1.0, 0.0);
	glTranslatef (2.0, 0.0, 0.0);
	glRotatef(rotation*4, 0.0, 1.0, 0.0);
	glutWireTeapot (0.6);

	glPopMatrix();

	glFlush ();
}

void idle() {
	clock_t wait;

	/* a better way to wait: */
	wait = clock () + NUM_TICKS;
	/* please wait...*/
	while (clock () < wait);

	rotation += ROTATION_INCR;

	glutPostRedisplay();       /* call Display function again */
}

void init ()
{
	glClearColor (0.8, 0.8, 0.8, 0.0);
	glShadeModel (GL_FLAT);
	glEnable (GL_DEPTH_TEST);
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	/* set up matrices for projection coordinate system */
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glFrustum (-1, 1, -1, 1, 1.5, 20.0);

	/* reset matrices to user's coordinate system */
	glMatrixMode (GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case '8': z = z + 1;
					 glutPostRedisplay ();
					 break;
		case '2': z = z - 1;
					 glutPostRedisplay ();
					 break;
		case 'q': exit (1);
	}
}

int main (int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Spinning Teapot");
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc (reshape);
	glutKeyboardFunc (keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0; 
}
