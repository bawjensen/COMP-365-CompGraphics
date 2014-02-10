// windows.cpp
// Demo for window/viewport functions using same circles program
// as shown previously.  The circles are now scaled twice:
// once because the window size is 0-400 and the output window is only 300x300
// and again because only a portion of the device window is used to draw
// as defined by the viewport function.  Note that the viewport function
// must appear in the display(); it does nothing if done in init()

#include <stdio.h>
#include <GL/glut.h>

void display ()
{
   int i;
   /* circles are made from quadric objects */
   GLUquadricObj *myobject;   
   GLUquadricObj *myobject2;
   GLUquadricObj *myobject3;

   myobject = gluNewQuadric();
   myobject2 = gluNewQuadric();
   myobject3 = gluNewQuadric();

   glClear (GL_COLOR_BUFFER_BIT);

   /* draw only from 0, 0 to 100, 100 in device window */
   /* (see what happens if this is taken out (or changed to 300, 300)) */
   glViewport (0, 0, 100, 100);   // (x, y, width, height)

   glColor3f (1.0, 1.0, 1.0);
   glBegin (GL_POINTS);
      for (i = 10; i < 400; i+=10)
         glVertex2i (i, 200);
      for (i = 0; i < 400; i++)
         glVertex2i (i, 400-i);
   glEnd();

   glTranslatef (80.0, 80.0, 0.0);  

   gluDisk (myobject, 0.0, 80.0, 100, 4);

   glTranslatef (120.0, 120.0, 0.0);
   glColor3f (0.0, 1.0, 0.0);
   gluQuadricDrawStyle (myobject2, GLU_LINE);  /* wireframe */
   gluDisk (myobject2, 40.0, 80.0, 10, 4);

   glTranslatef (80.0, 80.0, 0.0);
   gluQuadricDrawStyle (myobject3, GLU_LINE);  /* wireframe */
   glColor3f (0.0, 0.0, 1.0);
   gluDisk (myobject3, 0.0, 80.0, 60, 1);

   glTranslatef (-280.0, -280.0, 0.0);

   glFlush ();
}

void init ()
{
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
   glClearColor (0.0, 0.0, 0.0, 0.0);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   


   /* this maps 0-400 into the device window (which is 300x300) */
   /* (see what happens if you change this to 0-800) */
   glOrtho (0.0, 400.0, 0.0, 400.0, -1.0, 1.0);
   //gluOrtho2D (0.0, 400.0, 0.0, 400.0);
}

int main (int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   /* make the window square so that aspect ration is correct */
   glutInitWindowSize (300, 300); 

   glutInitWindowPosition (100, 100);

   glutCreateWindow ("windows/viewports");

   init ();

   glutDisplayFunc(display); 

   glutMainLoop();

   return 0; 
}

