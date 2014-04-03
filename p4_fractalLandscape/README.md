**PLANT CREATOR: USER GUIDE**
=============================
##### Bryan Jensen and Devin Delfino
------------------------------------
## __Description__
Generate a variety of plants on a landscape scene using the default grammars or custom grammars.

## __Included Files__

* **classes.h** : Header file for the various classes used in the program

* **classes.cpp** : Gives functionality to methods of the objects defined in classes.h

* **landscape.cpp** : Main file of C++ program

* **small.dem.grd** : Digital Elevation Model (in ESRI Format) representing a fractal landscape 

* **tree.gram** : Grammar creates an instance of a tree

* **bush.gram** : Grammar creates an instance of a bush

* **twig.gram** : Grammar creates an instance of a flower twig

* **README.md** : This user guide that includes the description of                          how to use the program

* **Makefile** : Makefile used to compile the program that is compatible with Linux and OS X operating systems

## __How to Compile__

* To compile the program using the included Makefile, use the command: </br>' ~$ make landscape '

* The executable file is named ' Delfino_Jensen_G4 ' and can be run with the command:</br>' ~$ ./Delfino_Jensen_G4 '

* To include additional grammar files, include the desired filename during your execute command as seen here:</br>' ~$ ./Delfino_Jensen_G4 *customGrammar1.gram*  '

* The program accepts up to and including 7 custom input files.

* If no input file is included, only the three provided grammars will be available

## __Program Controls__
####Keyboard Controls

* **space** - Move camera up
* **x** - Move camera down
* **s | 2** : Move camera backwards
* **w | 8** : Move camera forwards
* **a | 4** : Move camera left
* **d | 6** : Move camera right
* **esc** : Quit Program

####Mouse Controls
* **Left Click (and Hold)** : Pan the camera while moving the mouse with the left button held 
* **Left Click (on Minimap)** : Grow a plant using the current grammar at the indicated location
* **Right Click** : Open menu options

####Menu Options
* **Build Default Scene** : Builds a default scene including all three default types of plants
* **Randomly Place 3 of the Current Plant** : Randomly places 3 instances of the current plant type on the landscape
* **Randomly Place 5 Random Plants** : Randomly places 5 random plants on the landscape
* **Clear the Scene** : Clears the scene of all current plants
* **Choose Grammar (Tree)** : Changes the current grammar to display a tree
* **Choose Grammar (Bush)** : Changes the current grammar to display a bush
* **Choose Grammar (Flower Twig)** : Changes the current grammar to display a flower twig
* **Choose Custom Grammar #** : Changes the current grammar to use designated input file
* **Quit** : Quits program


## __Input File Format__
* The first line of the input file indicates the order of the plant, meaning how many iterations will it take to generate the plant. This line is made up of a character followed by an integer (ex. 'n 6')

* The second line of the input file indicates the rotation angle of the branches. This line is made up of a character followed by a float (ex. 'a 22.5')

* The third line of the input file indicates the starting length of a branch. This line consists of a character followed by a float (ex. 'l 10')

* The fourth line of the input file indicates the starting plant string. This line consists of a character followed by a string (ex. 's A' or 's ABA')

* To include a rotation in the grammar, the symbols combination is a Y (+/-) followed by an X (+/-)</br> Examples include *'Y+X+'*, *'Y+X-'*, *'Y-X+'*, *'Y-X-'*

* Note: The Y (+/-) MUST come before the X (+/-) 

* For every **[** symbol, there must be a corresponding **]** symbol

## __Valid Grammar Symbols__
* **B** : Creates a branch of the current color and size depending on the current location in the plant
* **L** : Creates a green leaf at the current location in the plant
* **F** : Creates a yellow flower at the current location in the plant
* **E** : Creates a purple berry at the current location in the plant 
* **G** : Increments the green value in the red, green, blue (rgb) color format  
* **g** : Decrements the green value in the red, green, blue (rgb) color format  
* **X** : Indicates a rotation about the X axis
* **Y** : Indicates a rotation about the Y axis
* **+** : 
* **-** : 
* **[** : 
* **]** : 

## __Available Plant Parts__
* **Branch** : A brown branch that will be scaled down in size the higher its location is on the plant
* **Leaf** : A green leaf that remains the same size throughout the plant
* **Flower** : A yellow flower that remains the same size throughout the plant
* **Berry** : A purple berry that remains the same size throughout the plant

## __Sample Input Files__

## __Sample Output__
