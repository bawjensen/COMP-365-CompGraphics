#ifndef _TEXTURES
#define _TEXTURES

#include "constants.h"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __linux
	#include <GL/glut.h>
#endif

static unsigned int texture[1]; // Array of texture indices.

// Struct of bitmap file.
struct BitMapFile
{
	int sizeX;
	int sizeY;
	unsigned char *data;
};

BitMapFile *getBMPData(string filename); // Function to get the BMP data from the image
void loadExternalTextures(); // Loads the external images
void initTextures(); // The initializer function for the textures
void drawTexturedMap(); // The draw function for the original map

#endif