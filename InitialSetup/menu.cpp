// menu.cpp
// Demo showing use of easy menus

#include <GL/glut.h>
#include <iostream>
#include <cstring>

using namespace std;

static int lastClickX = 0;
static int lastClickY = 0;
static int windowHeight = 500;
static int windowWidth = 500;

void mymenu (int);   // menu callback function

void display ()
{
   glClear (GL_COLOR_BUFFER_BIT);

   glFlush ();
}

void init ()
{
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
   glClearColor (0.0, 0.0, 0.8, 0.0);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   
   gluOrtho2D (0.0, 500, 0.0, 500);
}

void myMouse(int button, int state, int x, int y) {
   if (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
      cout << "Old click at - X: " << lastClickX << " Y: " << lastClickY << endl;
      lastClickX = x;
      lastClickY = y;
      cout << "New click at - X: " << x << " Y: " << y << endl;
   }
}

void initmenu ()
// set up menu
{
   glutCreateMenu (mymenu);       // register callback function
   glutAddMenuEntry ("Menu", 0);
   glutAddMenuEntry ("", 0);
   glutAddMenuEntry ("Hello", 1);
   glutAddMenuEntry ("Goodbye", 2);
   glutAttachMenu (GLUT_RIGHT_BUTTON);
}

void mymenu (int value)
// Define actions for the menu based on values given in initmenu()
{
   if (value == 1) {
      string output = "Hey!";
      cout << "Drawing at: " << lastClickX << ", " << lastClickY << endl;
      glRasterPos2i (lastClickX, windowHeight-lastClickY);
      for (int i = 0; i < output.length(); i++) {
         glutBitmapCharacter (GLUT_BITMAP_9_BY_15, output[i]);
      }
   }

   // switch (value) {
   //    case 1: cout << "Hey there, hello!" << endl;
   //            break;
   //    case 2: cout << "Goodbye" << endl;
   //            exit (1);
   // }
}

int main (int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutCreateWindow ("lil menu");
   init ();

   initmenu ();

   glutMouseFunc(myMouse);
   glutDisplayFunc(display); 
   glutMainLoop();

   return 0; 
}