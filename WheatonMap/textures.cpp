#include "constants.cpp"

using namespace std;

static unsigned int texture[1]; // Array of texture indices.

// Struct of bitmap file.
struct BitMapFile
{
	int sizeX;
	int sizeY;
	unsigned char *data;
};

// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile *getBMPData(string filename) {
	BitMapFile *bmp = new BitMapFile;
	unsigned int size, offset, headerSize;

	// Read input file name.
	ifstream infile(filename.c_str(), ios::binary);

	// Get the starting point of the image data.
	infile.seekg(10);
	infile.read((char *) &offset, 4); 

	// Get the header size of the bitmap.
	infile.read((char *) &headerSize,4);

	// Get width and height values in the bitmap header.
	infile.seekg(18);
	infile.read( (char *) &bmp->sizeX, 4);
	infile.read( (char *) &bmp->sizeY, 4);

	// Allocate buffer for the image.
	size = bmp->sizeX * bmp->sizeY * 24;
	bmp->data = new unsigned char[size];

	// Read bitmap data.
	infile.seekg(offset);
	infile.read((char *) bmp->data , size);

	// Reverse color from bgr to rgb.
	int temp;
	for (int i = 0; i < size; i += 3) {
		temp = bmp->data[i];
		bmp->data[i] = bmp->data[i+2];
		bmp->data[i+2] = temp;
	}

	return bmp;
}

// Load external textures.
void loadExternalTextures()			
{
	// Local storage for bmp image data.
	BitMapFile *image[2];

	// Load the textures.
	image[0] = getBMPData("wheatonMap.bmp");

	// Bind grass image to texture index[0]. 
	glBindTexture(GL_TEXTURE_2D, texture[0]); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
				 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);		
}

void initTextures() {
	glGenTextures(1, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void drawTexturedMap() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);        
	glBegin(GL_POLYGON);
	// glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.0, 0.0); glVertex2i(0, 0);
		glTexCoord2f(1.0, 0.0); glVertex2i(mapWidth, 0);
		glTexCoord2f(1.0, 1.0); glVertex2i(mapWidth, mapHeight);
		glTexCoord2f(0.0, 1.0); glVertex2i(0, mapHeight);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}