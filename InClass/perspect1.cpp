/* perspect1.cpp
 *
 * Shows how to use frustum 
 * (taken from page 98 in OpenGL (1.1) text or page 100 in OpenGL (1.2) text) 
 * User can change the value of the eye position by pressing 8 (moving
 * farther away) and 2 (moving closer) on keyboard.
 *
 * Experiment with this code be creating different polygons and changing
 * the center of projection position until you are sure you understand how
 * the frustum works.
 */

#include <iostream>
#include <GL/glut.h>

using namespace std;

float eyeX = 0.0;  // distance of eye (center of projection) from frustum
float eyeY = 0.0;  // distance of eye (center of projection) from frustum
float eyeZ = 5.0;  // distance of eye (center of projection) from frustum

void display ()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();

	gluLookAt (eyeX, 0.0, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	/*         -----------  -------------  -------------
	*         |            |              +-> view up vector, here signifying
	*         |            |                  that +y is up (default)
	*         |            +-> view reference point - "look-at point",
	*         |                usually near the center of the scene
	*         +-> origin of viewing reference frame - the "camera" position,
	*             pointing towards the look-at point
	*/

	/* scale objects here (if needed) to make sure they fit into frustum */
	glScalef (1.0, 1.0, 1.0);

	glColor3f (1.0, 0.0, 0.0);
	/* draw a polygon */
	glBegin (GL_POLYGON);
		glVertex3f (-0.5, 0.0, 1.0);
		glVertex3f (0.0, 0.5, 1.0);
		glVertex3f (0.5, 0.0, 1.0);
	glEnd();

	glBegin (GL_POLYGON);
		glVertex3f (0.0, 1.0, 0.0);
		glVertex3f (0.0, 1.0, 1.0);
		glVertex3f (1.0, 1.0, 1.0);
		glVertex3f (1.0, 1.0, 0.0);
	glEnd();

	glColor3f (0.0, 0.0, 1.0);
	/* draw a unit cube (centered at 0,0,0) */
	glutSolidCube (1.0);

	glFlush ();
}

void init ()
{
	glClearColor (0.8, 0.8, 0.8, 0.0);
	glShadeModel (GL_FLAT);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
}

void reshape (int w, int h)
{
	 glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	 /* set up matrices for projection coordinate system */
	 glMatrixMode (GL_PROJECTION);
	 glLoadIdentity ();
	 /* Frustum normally defined as -1 to 1 in x, y and a larger spread in z */
	 glFrustum (-1, 1, -1, 1, 1.5, 20.0);



/***************************************************

Remember this is a right-handed system, so z is like this:

+z <-------------------------------------------------------> -z  screen is here

With the call to gluLookAt(), the "eye" (center of projection) is set at 5 in z:

					 O
+z <-------------------------------------------------------> -z  screen is here
					+5

Frustum is set up so that the near plane is A DISTANCE of 1.5 from the eye
position and the far plane is A DISTANCE of 20.0 from the eye:

																		 | far plane
							 | near plane          | 
					 O   |                     |
+z <-------------------------------------------------------> -z  screen is here
					+5  3.5                   -15
					 ^---^
						1.5
					 ^-------------------------^
										 20.0


This means that the objects must be between 3.5 and -15 in Z (see triangle
polygon above, for example). 

***************************************************/

	 /* reset matrices to user's coordinate system */
	 glMatrixMode (GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y) {
	 switch (key) {
		case '8': eyeZ = eyeZ + 1;
				cout << "eyeZ = " << eyeZ << endl;
				glutPostRedisplay();
				break;
		case '2': eyeZ = eyeZ - 1;
				cout << "eyeZ = " << eyeZ << endl;
				glutPostRedisplay();
				break;
		case '4': eyeX = eyeX + 1;
				cout << "eyeX = " << eyeX << endl;
				glutPostRedisplay();
				break;
		case '6': eyeX = eyeX - 1;
				cout << "eyeX = " << eyeX << endl;
				glutPostRedisplay();
				break;
		case 27:
		case 'q': exit (1);
	 }
}

void specialKeyboard(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP: keyboard('8', x, y);
				break;
		case GLUT_KEY_DOWN: keyboard('2', x, y);
				break;
		case GLUT_KEY_RIGHT: keyboard('6', x, y);
				break;
		case GLUT_KEY_LEFT: keyboard('4', x, y);
				break;
	}
}

int main (int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Frustum Fun");
	init();
	glutDisplayFunc(display); 
	glutReshapeFunc (reshape);
	glutKeyboardFunc (keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMainLoop();
	return 0; 
}