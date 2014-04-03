**PLANT CREATOR: USER GUIDE**
=============================
##### Bryan Jensen and Devin Delfino
------------------------------------
## __Description__
Generate a variety of plants on a landscape scene.

## __Included Files__

* *classes.h* - Header file for the various classes used in the program

* *classes.cpp* - Gives functionality to methods of the objects defined in classes.h

* *landscape.cpp* - Main file of C++ program
* *small.dem.grd* - Digital Elevation Model (in ESRI Format) representing a fractal landscape 

* *test.gram* - Grammar creates an instance of a tree

* *README.md* - This user guide that includes the description of                          how to use the program

* *Makefile* - Makefile used to compile the program that is compatible with Linux and OS X operating systems

## __How to Compile__

* To compile the program using the included Makefile, use the command: </br>' ~$ make landscape '

* The executable file is named ' Delfino_Jensen_G4 ' and can be run with the command:</br>' ~$ ./Delfino_Jensen_G4 '

* To include additional grammar files, include the desired filename during your execute command as seen here:</br>' ~$ ./Delfino_Jensen_G4 *customGrammar1.gram*  '
 
* If no input file is included, only the three provided grammars will be available

## __Program Controls__
####Keyboard Controls

* *space* - Move camera up
* *x* - Move camera down
* *s | 2* - Move camera backwards
* *w | 8* - Move camera forwards
* *a | 4* - Move camera left
* *d | 6* - Move camera right
* *esc* - Quit Program

####Mouse Controls
* *Left Click (and Hold)* - Pan the camera while moving the mouse with the left button held 
* *Left Click (on Minimap)* - Grow a plant using the current grammar at the indicated location
* *Right Click* - Open menu options

####Menu Options
* *Randomly Place 5 Random Plants* - 
* *Randomly Place 
## __Input File Format__

## __Valid Grammar Symbols__
* *B* - B creates a branch of the current color and size depending on its location in the plant
* *L* - L creates a green leaf at the current location
* *F* - F creates a 
* *E* - E creates a purple berry 
* *G* - G increments 
* *X* - X indicates a rotation about the X axis
* *Y* - Y indicates a rotation about the Y axis
* *+* - + increments the 
* *-* - - increments the 
* *[* - 
* *]* - 

## __Available Plant Parts__
* Branch - A brown branch that will be scaled down in size the higher its location is on the plant
* Leaf - A green leaf that remains the same size throughout the plant
* Flower - 
* Berry - A purple berry that remains the same size throughout the plant

## __Sample Input Files__

## __Sample Output__
