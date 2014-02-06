#ifndef _TEXTURES
#define _TEXTURES

#include "constants.h"

#include <GL/glut.h>

static unsigned int texture[1]; // Array of texture indices.

// Struct of bitmap file.
struct BitMapFile
{
	int sizeX;
	int sizeY;
	unsigned char *data;
};

BitMapFile *getBMPData(string filename);
void loadExternalTextures();		
void initTextures();
void drawTexturedMap();

#endif