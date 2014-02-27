#ifndef _CONSTANTS
#define _CONSTANTS

using namespace std;

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

static const int KEY_ESCAPE = 27; // ASCII key code for escape
static const int MOUSE_SCROLL_UP = 3; // Key code for mouse scrolling up
static const int MOUSE_SCROLL_DOWN = 4; // Key code for mouse scrolling down

// Building type constants:
static const int BUILDING_GENERAL = 100; // General purpose college building (academic, etc.)
static const int BUILDING_DORM = 101; // Dorms and other student housing
static const int BUILDING_ROAD = 102; // Roads and driveways
static const int BUILDING_FIELD = 103; // Fields
static const int BUILDING_PARKINGLOT = 104; // Parking lots and some driveways
static const int BUILDING_WATER = 105; // Water (aka Peacock Pond)

static const string filename = "buildings.txt"; // File name for the saved mapped buildings
static const int mapWidth = 827; // Width of the original map, on which the plot was based
static const int mapHeight = 684; // Width of the original map, on which the plot was based

#endif