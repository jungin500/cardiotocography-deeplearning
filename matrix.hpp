#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

namespace Cardiography {

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

		// Constructor
		Matrix(int row, int col) : row(row), col(col) {
			item = new T*[row];
			for (int i = 0; i < row; i++)
				item[i] = new T[col];
			fillRandomNumber();
		}

		Matrix(int row, int col, T *data[]) : Matrix(row, col) {
			// assert(sizeof(data) == sizeof(T) * row);
			item = data;
		}

		Matrix(const Matrix<T> &another) {
			row = another.row;
			col = another.col;
			item = new T*[row];
			for (int i = 0; i < row; i++) {
				item[i] = new T[col];
				for (int j = 0; j < col; j++)
					item[i][j] = another.item[i][j];
			}
		}


		void fillRandomNumber() {
			srand((unsigned int)time(NULL));
			int delim, sign;
			double val;

			for (int j = 0; j < row; j++) {
				for (int i = 0; i < col; i++) {
					delim = rand();

					if (delim != 0) {
						sign = (rand() % 2 - 1);
						val = (double)(rand() % delim) / delim / 20.0;
						item[j][i] = sign * val;
					}
					else
						item[j][i] = 0.0;
				}
			}
		}

		T at(int col, int row) {
			assert(0 <= row && row < this->row);
			assert(0 <= col && col < this->col);
			return item[col][row];
		}

		void each(double(*fp)(double)) {
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++)
					item[i][j] = fp(item[i][j]);
		}

		void set(int col, int row, T data) {
			at(col, row);
			item[col][row] = data;
		}

		Size size() {
			Size sz;
			sz.rows = row;
			sz.cols = col;
			return sz;
		}

		void replaceItem(int col, int row, T **data) {
			this->data = data;
			this->col = col;
			this->row = row;
		}

		Matrix<T> operator* (const Matrix<T> &another) {
			if (another.row != col)
				throw new std::exception("Matrix multiplication failure: Invalid multipliction");

			Matrix<T> result(row, another.col);
			for (int j = 0; j < row; j++)
				for (int i = 0; i < another.col; i++) {
					T resultItem = 0.;

					for (int this_cols = 0; this_cols < col; this_cols++)
						resultItem += item[j][this_cols] * another.item[this_cols][i];

					result.item[j][i] = resultItem; // TODO: Check accessor failure of template6
				}

			return result;
		}

		void print(const Matrix<T> &mat) {
			for (int j = 0; j < mat.row; j++) {
				for (int i = 0; i < mat.col; i++) {
					cout << mat.item[j][i];
					if (i != mat.col - 1)
						cout << ",\t";
				}
				cout << "\n";
			}
		}

		void print() {
			print(*this);
		}
	};
}