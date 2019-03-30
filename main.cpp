#include "csv.hpp"
#include "matrix.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace Cardiography;

/* 
int main() {
	CSVReader reader("dataset/cardiography-dataset.csv");
	in.read_header(io::ignore_extra_column, "LB", "AC", "FM", "UC",
		"DL", "DS", "DP", "ASTV", "MSTV", "ALTV", "MLTV", "Width",
		"Min", "Max", "Nmax", "Nzeros", "Mode", "Mean", "Median",
		"Variance", "Tendency", "NSP");

	// Define data of columns
	double row_data[22] = { 0., };

	while (in.read_row(row_data)) {
		// Do each row
		Matrix<double> newMatrix(4, 5);
		cout << newMatrix.size().rows << ", " << newMatrix.size().cols << endl;
	}
}
*/

void printArr(double *arr[], int rows, int cols) {
	for (int j = 0; j < rows; j++) {
		for (int i = 0; i < cols; i++) {
			cout << arr[j][i];
			if (i != cols - 1)
				cout << ",\t";
		}
		cout << "\n";
	}
}

int main() {
	CSVReader reader("dataset/cardiography-dataset.csv");
	cout << reader.nextLine() << endl;
}