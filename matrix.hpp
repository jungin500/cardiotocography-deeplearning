#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

using namespace std;

namespace Cardiotocography {

	struct Size {
		int rows;
		int cols;
	};

	template<typename T = double>
	class Matrix {
	private:
		T** item;
		int row;
		int col;

	public:

		// Constructor
		Matrix(int row, int col) : row(row), col(col) {
			item = new T * [row];
			for (int i = 0; i < row; i++)
				item[i] = new T[col];
			fillRandomNumber();
		}

		Matrix(int row, int col, T * data[]) : Matrix(row, col) {
			// assert(sizeof(data) == sizeof(T) * row);
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++)
					item[i][j] = data[i][j];
		}

		~Matrix() {
			for (int i = 0; i < row; i++)
				delete[] item[i];
			delete[] item;
		}

		Matrix(const Matrix<T> & another) {
			row = another.row;
			col = another.col;
			item = new T * [row];
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

		T at(int row, int col) {
			// assert(row < this->row && col < this->col)
			return item[row][col];
		}

		void each(double(*fp)(double)) {
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++)
					item[i][j] = fp(item[i][j]);
		}

		void set(int row, int col, T data) {
			item[row][col] = data;
		}

		Size size() {
			Size sz;
			sz.rows = row;
			sz.cols = col;
			return sz;
		}

		Matrix<T> operator* (const Matrix<T> & another) {
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

		void print(const Matrix<T> & mat) {
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

#pragma region Mathematical Calculation
		Matrix<T>* squaredError(const Matrix<T> * correct_one_hot) {
			// assert(correct_one_hot->row == 1 && row == 1 && correct_one_hot->col == col);

			Matrix<T>* error = new Matrix<T>(1, col);
			for (int i = 0; i < col; i++)
				error.item[0][i] = pow(correct_one_hot->item[0][i] - item[0][i], 2);

			return error;
		}

		Matrix<T>* differenceError(const Matrix<T> * correct_one_hot) {
			Matrix<T>* error = new Matrix<T>(1, col);
			for (int i = 0; i < col; i++)
				error->item[0][i] = correct_one_hot->item[0][i] - item[0][i];

			return error;
		}

		Matrix<T> transpose() {
			Matrix<T> transposeMatrix(col, row);
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++)
					transposeMatrix.item[j][i] = item[i][j];
			return transposeMatrix;
		}

		// Mean squared Error. Suppose row == 1 and col == 3?
		double mse(const Matrix<T> * correct_one_hot) {
			double mse = 0;
			for (int i = 0; i < col; i++)
				mse += pow(item[0][i] - correct_one_hot->item[0][i], 2) / 2;
			return mse;
		}

		double max() {
			double maxValue = item[0][0];
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++)
					maxValue = std::max(maxValue, item[i][j]);
			return maxValue;
		}

		int maxIndexAsOneHot() {
			double maxValue = item[0][0]; int maxIndex = 0;
			for (int j = 0; j < col; j++) {
				maxValue = std::max(maxValue, item[0][j]);
				if (maxValue == item[0][j])
					maxIndex = j;
			}
			return maxIndex;
		}
#pragma endregion

#pragma region Static Mathematical Calculation

		// Returns negative of each elements
		static Matrix<T>* neg(const Matrix<T> * original) {
			Matrix<T>* negMat = new Matrix<T>(original->row, original->col);

			for (int i = 0; i < original->row; i++)
				for (int j = 0; j < original->col; j++)
					negMat->item[i][j] = -(original->item[i][j]);

			return negMat;
		}
#pragma endregion
	};
}