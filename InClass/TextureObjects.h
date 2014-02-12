#ifndef _TEXTUREOBJECTS_H
#define _TEXTUREOBJECTS_H

/*********************************************************************************************\
|	
|	Mark M. Lanning
|	Starlight Software Co.
|	
|	TextureObjects.h
|		these is the header for the TextureObjects classes that work with glaux to load bmp textures
|
\*********************************************************************************************/

#pragma comment(lib, "glaux.lib")
#include <GL\glaux.h>

#pragma comment(lib, "jpeg.lib")
#include "jpeglib.h"

const bool DEBUG = true;			//set debug so we get feedback about what is going on



//Define a new structure to hold the image information
typedef struct _ImageDataRec {
	int rowSpan;
	int sizeX;
	int sizeY;
	unsigned char *data;
} ImageDataRec;

/*
//Old Structure information
typedef struct _AUX_RGBImageRec {
	GLint sizeX, 
	GLint sizeY;
	unsigned char *data;
} AUX_RGBImageRec;

struct tImageJPG
{
	int rowSpan;
	int sizeX;
	int sizeY;
	unsigned char *data;
};
*/	

class TextureSystem
{
	public:

		// Typical constructor
		TextureSystem();

		// This will clean up memory and close the sound system
		~TextureSystem();

		// This will load the texture from the file and put it into the passed texture
		bool LoadTexture(UINT &mTexture, char *mFileName, bool IsJPG);

	private:
		// This checks to make sure the file exists
		bool FileExist(char *mFileName);
		
};

#endif
