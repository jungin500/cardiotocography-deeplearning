#include "dataset/csv.h"
#include "matrix.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace matrix;

/*
int main() {
	io::CSVReader<22> in("CTG.csv");
	in.read_header(io::ignore_extra_column, "LB", "AC", "FM", "UC",
		"DL", "DS", "DP", "ASTV", "MSTV", "ALTV", "MLTV", "Width",
		"Min", "Max", "Nmax", "Nzeros", "Mode", "Mean", "Median",
		"Variance", "Tendency", "NSP");

	// Define data of columns
	double row_data[22] = { 0., };

	while (in.read_row(row_data)) {
		// Do each row
		Matrix::Matrix<double> newMatrix(4, 5);
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
	double *arr1[1] = { new double[3] { 1.,  1.,  1. } }; // 1, 3
	double *arr2[3] = { new double[2] { 1., 1. }, new double[2] { 1., 1. }, new double[2] { 1., 1. } }; // 3, 2

	Matrix<double> mat1(1, 3, arr1);
	Matrix<double> mat2(3, 2, arr2);

	Matrix<double> mat3 = mat1 * mat2;
	mat3.print();

	return 0;
}