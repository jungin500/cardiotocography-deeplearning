#include "csv.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

namespace Matrix {

	struct Size {
		int rows;
		int cols;
	};

	template<typename T = double>
	class Matrix {
	private:
		T **item;
		int row;
		int col;

	public:

		Matrix(int row, int col) : row(row), col(col) {
			*item = new T[col];
			for (int i = 0; i < col; i++)
				item[i] = new T[row];
		}

		void fillRandomNumber() {
			srand((unsigned int)time(NULL));
			int delim = rand();

			for (int i = 0; i < col; i++) {
				for (int j = 0; j < row; j++)
					item[i][j] = (rand() % delim) / delim / 20;
			}
		}

		T at(int col, int row) {
			return item[col][row];
		}

		Size size() {
			Size sz;
			sz.rows = row;
			sz.cols = col;
			return sz;
		}

		Matrix<T> operator*(Matrix<T> another) {
			if (another.row != col)
				throw new std::exception("Matrix multiplication failure: Invalid multipliction");

			Matrix<T> result(row, another.col);
			for (int j = 0; j < row; j++)
				for (int i = 0; i < another.col; i++) {
					T resultItem = 0.;

					for (int this_cols = 0; this_cols < col; this_cols++)
						for (int another_rows = 0; another_rows < another.row; another_rows++)
							resultItem += item[this_cols][j] * another.item[i][another_rows];

					result.item[j][i] = resultItem; // TODO: Check accessor failure of template
				}

			return result;
		}
	};
}

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
