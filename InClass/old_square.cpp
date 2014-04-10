////////////////////////////////////////////////////          
// square.cpp
//
// Stripped down OpenGL program that draws a square.
// 
// Sumanta Guha.
////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

float array[1000] = {0};
int currI = 0;
int state = GL_POLYGON;

void addPoint(int x, int y) {
   cout << "Adding point at: " << x/5 << ", " << (500-y)/5 << endl;
   array[currI++] = x/5;
   array[currI++] = (500-y)/5;
   glutPostRedisplay();
}

// Drawing (display) routine.
void drawScene(void)
{
   // Clear screen to background color.
   glClear(GL_COLOR_BUFFER_BIT);

   // Set foreground (or drawing) color.
   glBegin(state);
   glColor3f(0.0, 0.0, 1.0);


   for (int i = 0; i < 1000; i += 2) {
      if (array[i] != 0)
         glVertex3f(array[i], array[i+1], 0.0);
   }
   
   glEnd();

   // Draw a polygon with specified vertices.

   // Flush created objects to the screen, i.e., force rendering.
   glFlush(); 
}

// Initialization routine.
void setup(void) 
{
   // Set background (or clearing) color.
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   ifstream inFile;
   float x, y;

   inFile.open("points.txt");

   if (!inFile) exit(1);
   int i = 0;

   inFile >> x >> y;
   while (inFile) {
      array[i++] = x;
      array[i++] = y;
      inFile >> x >> y;
   }

   currI = i;
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   // Set viewport size to be entire OpenGL window.
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  
   // Set matrix mode to projection.
   glMatrixMode(GL_PROJECTION);

   // Clear current projection matrix to identity.
   glLoadIdentity();

   // Specify the orthographic (or perpendicular) projection, 
   // i.e., define the viewing box.
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);

   // Set matrix mode to modelview.
   glMatrixMode(GL_MODELVIEW);

   // Clear current modelview matrix to identity.
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
	  // Press escape to exit.
      case 'a':
         addPoint(x, y);
         break;
      case 'z':
         state = GL_POLYGON;
         break;
      case 'x':
         state = GL_LINE_STRIP;
         break;

      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Main routine: defines window properties, creates window,
// registers callback routines and begins processing.
int main(int argc, char **argv) 
{  
   // Initialize GLUT.
   glutInit(&argc, argv);
 
   // Set display mode as single-buffered and RGB color.
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   
   // Set OpenGL window size.
   glutInitWindowSize(500, 500);

   // Set position of OpenGL window upper-left corner.
   glutInitWindowPosition(100, 100); 
   
   // Create OpenGL window with title.
   glutCreateWindow("square.cpp");
   
   // Initialize.
   setup(); 
   
   // Register display routine.
   glutDisplayFunc(drawScene); 
   glutIdleFunc(drawScene); 
   
   // Register reshape routine.
   glutReshapeFunc(resize);  

   // Register keyboard routine.
   glutKeyboardFunc(keyInput);
   
   // Begin processing.
   glutMainLoop(); 

   return 0;  
}