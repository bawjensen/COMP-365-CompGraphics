/* multTexture.cpp
 *
 * Texture mapping starting from mytexture.cpp.
 * Draws multiple textures onto a parallelpiped.
 * 
 * Follow steps (0) to create multiple textures.
 */

#include <iostream>
#include <GLUT/glut.h>
#include "glm.h"	// find in /tutors

void rectSolid ();
void mymenu (int);

using namespace std;

float xPos = 0.0;   // x position of camera
float yPos = 0.0;   // y position of camera
float zPos = 3.0;   // distance the center of projection from frustum 
int wireframe = 0;  // toggle for wire-frame
int depthbuff = 1;  // toggle for depth buffer
int texture = 1;    // toggle for texture
unsigned char* image = NULL;   // store data of texture map
unsigned char* image2 = NULL;  // store data of second texture map
unsigned char* image3 = NULL;  // store data of third texture map
int texWidth;
int texHeight;

// (1) create texture array
static unsigned int textureArray[4];	  // Array of textures

void display ()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity ();

   // Change position of camera in x, y, and z
   gluLookAt (xPos, yPos, zPos, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // draw solid object
   rectSolid ();

   glutSwapBuffers();
}

// Draw a rectangular solid centered at 0, 0, 0
void rectSolid ()
{
   float Xr = 0.5;	// right x
   float Xl = -0.5;	// left x
   float Yt = 0.5;	// top y
   float Yb = -0.5;	// bottom y
   float Zf = 0.5;	// front z
   float Zb = -0.5;	// back z

   glColor3f (0.2, 0.0, 1.0);

   if (texture)
      glEnable (GL_TEXTURE_2D);

   // (5) Bind the appropriate texture to desired polygon
   glBindTexture(GL_TEXTURE_2D, textureArray[1]);
   // right side (defined in counter-clockwise order)
   glBegin (GL_TRIANGLES);
      // must define texture BEFORE each vertex!!
      // Textures are defined 0,0 for bottom left
      glTexCoord2f (0, 0);
      glVertex3f (Xr, Yb, Zf);  // lower left

      glTexCoord2f (1, 0);
      glVertex3f (Xr, Yb, Zb);  // lower right

      glTexCoord2f (0.5, 1.0);
      glVertex3f (0.0, 0.0, 0.0);  // upper middle
   glEnd();

   // left side
   glBegin (GL_TRIANGLES);
      glTexCoord2f (0.5, 1.0);
      glVertex3f (0, 0, 0);
      glTexCoord2f (0, 0);
      glVertex3f (Xl, Yb, Zb);
      glTexCoord2f (1, 0);
      glVertex3f (Xl, Yb, Zf);
   glEnd();

   // bottom
   glBegin (GL_QUADS);
      glTexCoord2f (0, 0);
      glVertex3f (Xr, Yb, Zf);
      glTexCoord2f (1, 0);
      glVertex3f (Xl, Yb, Zf);
      glTexCoord2f (1, 1);
      glVertex3f (Xl, Yb, Zb);
      glTexCoord2f (0, 1);
      glVertex3f (Xr, Yb, Zb);
   glEnd();

   // back
   glBindTexture(GL_TEXTURE_2D, textureArray[0]);
   glColor3f (1.0, 0.5, 0.5);
   glBegin (GL_TRIANGLES);
      glTexCoord2f (0.5, 1.0);
      glVertex3f (0.0, 0.0, 0.0); // middle top
      glTexCoord2f (0, 0);
      glVertex3f (Xr, Yb, Zb); // lower right
      glTexCoord2f (1, 0);
      glVertex3f (Xl, Yb, Zb); // lower left
   glEnd();

   // front
   glColor3f (0.0, 0.5, 1.0);
   glBindTexture(GL_TEXTURE_2D, textureArray[2]);
   glBegin (GL_TRIANGLES);
      glTexCoord2f (0, 0);
      glVertex3f (Xl, Yb, Zf);  // lower left

      glTexCoord2f (1, 0);
      glVertex3f (Xr, Yb, Zf);  // lower right

      glTexCoord2f (0.5, 1);
      glVertex3f (0.0, 0.0, 0.0);  // upper right
   glEnd();

   if (texture)
      glDisable (GL_TEXTURE_2D);
} // rectSolid ()
   

void init ()
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable (GL_DEPTH_TEST);

   // (2) Let OpenGL bind textureArray
   // create texture array
   glGenTextures(2, textureArray);

   // (3) Define multiple textures

   // bind celeb image as texture #0
   char ch[20] = "CelebDMud.ppm\0";
   image=glmReadPPM (ch, &texWidth, &texHeight);
   // (4) Bind following texture to the texture array
   glBindTexture (GL_TEXTURE_2D, textureArray[0]);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexImage2D (GL_TEXTURE_2D, 	// target: 2D texture
		   0,			// level = 0 unless multiple resolutions
		   GL_RGB,		// internal image format 
		                        // (see OpenGL text for options)
		   texWidth,		// image width
		   texHeight,		// image height
		   0,			// border width (0 or 1;see OpenGL text)
		   GL_RGB,		// image format (see OpenGL text)
		   GL_UNSIGNED_BYTE,  	// format of data within image file
		   image);		// image file

   // bind newCeleb image as texture #1
   char ch2[20] = "checker.ppm\0";
   image2=glmReadPPM (ch2, &texWidth, &texHeight);
   glBindTexture (GL_TEXTURE_2D, textureArray[1]);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexImage2D (GL_TEXTURE_2D, 	// target: 2D texture
		   0,			// level = 0 unless multiple resolutions
		   GL_RGB,		// internal image format 
		                        // (see OpenGL text for options)
		   texWidth,		// image width
		   texHeight,		// image height
		   0,			// border width (0 or 1;see OpenGL text)
		   GL_RGB,		// image format (see OpenGL text)
		   GL_UNSIGNED_BYTE,  	// format of data within image file
		   image2);		// image file

   // bind newCeleb image as texture #1
   char ch3[20] = "train.ppm\0";
   image3=glmReadPPM (ch3, &texWidth, &texHeight);
   glBindTexture (GL_TEXTURE_2D, textureArray[2]);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexImage2D (GL_TEXTURE_2D,  // target: 2D texture
         0,       // level = 0 unless multiple resolutions
         GL_RGB,     // internal image format 
                              // (see OpenGL text for options)
         texWidth,      // image width
         texHeight,     // image height
         0,       // border width (0 or 1;see OpenGL text)
         GL_RGB,     // image format (see OpenGL text)
         GL_UNSIGNED_BYTE,    // format of data within image file
         image3);    // image file

}  // init ()

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1, 1, -1, 1, 2.0, 25.0);
   glMatrixMode (GL_MODELVIEW);
   glutSwapBuffers();
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case '-': zPos = zPos + 1;
                break;
      case '+': zPos = zPos - 1;
                break;
      case '4': xPos = xPos - 1;
                break;
      case '6': xPos = xPos + 1;
                break;
      case '8': yPos = yPos + 1;
                break;
      case '2': yPos = yPos - 1;
                break;
      case 'q': exit (1);
   }
   glutPostRedisplay ();
}

void initmenu ()
{
   glutCreateMenu (mymenu);
   glutAddMenuEntry ("Menu", 0);
   glutAddMenuEntry ("", 0);
   glutAddMenuEntry ("Toggle Wire-Frame", 1);
   glutAddMenuEntry ("Toggle Depth-Buffer", 2);
   glutAddMenuEntry ("Toggle Texture", 3);
   glutAddMenuEntry ("Quit", 6);
   glutAttachMenu (GLUT_LEFT_BUTTON);
   glutAttachMenu (GLUT_RIGHT_BUTTON);
}

void mymenu (int value)
// Define actions for menu options
{
   switch (value) {
      // toggle wire-frame; do both front- and rear-facing polygons
      case 1 : if (wireframe) {
		  glPolygonMode (GL_FRONT, GL_FILL);
		  glPolygonMode (GL_BACK, GL_FILL);
		  wireframe--;
	       } else {
		  glPolygonMode (GL_FRONT, GL_LINE);
		  glPolygonMode (GL_BACK, GL_LINE);
		  wireframe++;
	       }
	       break;
      // D  Toggle hidden-surface removal (z-buffer test)
      case 2 : if (depthbuff) {
		  glDisable (GL_DEPTH_TEST);
		  depthbuff--;
	       } else {
		  glEnable (GL_DEPTH_TEST);
		  depthbuff++;
	       }
	       break;
      case 3 : if (texture) 
	          texture--;
	       else
		  texture++;
	       break;
      case 6 : exit (1);
   }
   glutPostRedisplay();
} // mymenu ()


int main (int argc, char** argv) {
   glutInit(&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

   glutInitWindowSize (500, 500); 

   glutInitWindowPosition (100, 100);

   glutCreateWindow ("Multiple Textures");

   init ();

   initmenu ();

   glutDisplayFunc(display); 

   glutReshapeFunc (reshape);

   glutKeyboardFunc (keyboard);

   glutMainLoop();

   return 0; 
}
