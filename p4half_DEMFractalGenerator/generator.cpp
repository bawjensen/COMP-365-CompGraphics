#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include <fstream>

using namespace std;

string outFileName = "my3.dem.grd";
float roughnessFactor = 2.2;
default_random_engine generator;
normal_distribution<float> distribution(0.0, 0.5);


float randVal(int gridSize) {
	float rFactor = roughnessFactor;
	return rFactor * gridSize * distribution(generator);
}

void fractalRecurse(float** grid, int left, int right, int top, int bottom) {
	if ((right - left) <= 1) {
		return;
	}
	cout << "Recursing again: " << left << " to " << right << ", " << top << " to " << bottom << endl;

	int midPointH = (left + right) / 2;
	int midPointV = (top + bottom) / 2;

	grid[top][midPointH] = (grid[top][left] + grid[top][right]) / 2;
	grid[bottom][midPointH] = (grid[bottom][left] + grid[bottom][right]) / 2;
	grid[midPointV][left] = (grid[top][left] + grid[bottom][left]) / 2;
	grid[midPointV][right] = (grid[top][right] + grid[bottom][right]) / 2;

	grid[midPointV][midPointH] = (grid[top][left] + grid[top][right] + grid[bottom][left] + grid[bottom][right]) / 4 + randVal(left - right + 1);

	fractalRecurse(grid, left, midPointH, top, midPointV);
	fractalRecurse(grid, midPointH, right, top, midPointV);
	fractalRecurse(grid, left, midPointH, midPointV, bottom);
	fractalRecurse(grid, midPointH, right, midPointV, bottom);
}

float** generateGrid(int width) {
	float** grid = new float*[width];
	for (int i = 0; i < width; i++) grid[i] = new float[width];


	float scalar = 1;
	float shift = width - 1;
	for (int i = 0; i < width; i++) {
		grid[0][i] = i * scalar;
		grid[width-1][i] = shift + i * scalar;
		grid[i][0] = i * scalar;
		grid[i][width-1] = shift + i * scalar;
	}

	fractalRecurse(grid, 0, width - 1, 0, width - 1);

	return grid;

}

int main() {
	int n = 6;

	int width = pow(2, n) + 1;

	float** grid = generateGrid(width);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			cout << setw(10) << grid[i][j] << " ";
		}
		cout << endl;
	}

	ofstream outFile;

	outFile.open(outFileName.c_str());

	if (!outFile) {
		cout << "File " << outFileName << " doesn't exist" << endl;
		exit(1);
	}

	outFile << "ncols ";
	outFile << width;
	outFile << endl;

	outFile << "nrows ";
	outFile << width;
	outFile << endl;

	outFile << "xllcorner ";
	outFile << "0";
	outFile << endl;

	outFile << "yllcorner ";
	outFile << "0";
	outFile << endl;

	outFile << "cellsize ";
	outFile << "10";
	outFile << endl;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			outFile << grid[i][j] << " ";
		}
		outFile << endl;
	}
}