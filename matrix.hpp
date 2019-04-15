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
		T** _item;
		int _row;
		int _col;

	public:

		// Constructor
		Matrix(int row, int col) : _row(row), _col(col) {
			_item = new T * [row];
			for (int i = 0; i < row; i++)
				_item[i] = new T[col];
			fillRandomNumber();
		}

		Matrix(int row, int col, T * data[]) : Matrix(row, col) {
			// assert(sizeof(data) == sizeof(T) * row);
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++)
					_item[i][j] = data[i][j];
		}

		~Matrix() {
			for (int i = 0; i < _row; i++)
				delete[] _item[i];
			delete[] _item;
		}

		Matrix(const Matrix<T> & another) {
			_row = another._row;
			_col = another._col;
			_item = new T * [_row];
			for (int i = 0; i < _row; i++) {
				_item[i] = new T[_col];
				for (int j = 0; j < _col; j++)
					_item[i][j] = another._item[i][j];
			}
		}

		void fillRandomNumber() {
			srand((unsigned int)time(NULL));
			int delim, sign;
			double val;

			for (int j = 0; j < _row; j++) {
				for (int i = 0; i < _col; i++) {
					delim = rand();

					if (delim != 0) {
						sign = (rand() % 2 - 1);
						val = (double)(rand() % delim) / delim / 20.0;
						_item[j][i] = sign * val;
					}
					else
						_item[j][i] = 0.0;
				}
			}
		}

		T at(int row, int col) {
			// assert(row < this->row && col < this->col)
			return _item[row][col];
		}

		void each(double(*fp)(double)) {
			for (int i = 0; i < _row; i++)
				for (int j = 0; j < _col; j++)
					_item[i][j] = fp(_item[i][j]);
		}

		void set(int row, int col, T data) {
			_item[row][col] = data;
		}

		Size size() {
			Size sz;
			sz.rows = _row;
			sz.cols = _col;
			return sz;
		}

		Matrix<T> operator* (const Matrix<T> & another) {
			if (another._row != _col)
				throw new std::exception("Matrix multiplication failure: Invalid multipliction");

			Matrix<T> result(_row, another._col);
			for (int j = 0; j < _row; j++)
				for (int i = 0; i < another._col; i++) {
					T resultItem = 0.;

					for (int this_cols = 0; this_cols < _col; this_cols++)
						resultItem += _item[j][this_cols] * another._item[this_cols][i];

					result._item[j][i] = resultItem; // TODO: Check accessor failure of template6
				}

			return result;
		}

		void print(const Matrix<T> & mat) {
			for (int j = 0; j < mat._row; j++) {
				for (int i = 0; i < mat._col; i++) {
					cout << mat._item[j][i];
					if (i != mat._col - 1)
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

			Matrix<T>* error = new Matrix<T>(1, _col);
			for (int i = 0; i < _col; i++)
				error.item[0][i] = pow(correct_one_hot->_item[0][i] - _item[0][i], 2);

			return error;
		}

		Matrix<T>* differenceError(const Matrix<T> * correct_one_hot) {
			Matrix<T>* error = new Matrix<T>(1, _col);
			for (int i = 0; i < _col; i++)
				error->_item[0][i] = correct_one_hot->_item[0][i] - _item[0][i];

			return error;
		}

		Matrix<T> transpose() {
			Matrix<T> transposeMatrix(_col, _row);
			for (int i = 0; i < _row; i++)
				for (int j = 0; j < _col; j++)
					transposeMatrix._item[j][i] = _item[i][j];
			return transposeMatrix;
		}

		// Mean squared Error. Suppose row == 1 and col == 3?
		double mse(const Matrix<T> * correct_one_hot) {
			double mse = 0;
			for (int i = 0; i < _col; i++)
				mse += pow(_item[0][i] - correct_one_hot->_item[0][i], 2) / 2;
			return mse;
		}

		double max() {
			double maxValue = _item[0][0];
			for (int i = 0; i < _row; i++)
				for (int j = 0; j < _col; j++)
					maxValue = std::max(maxValue, _item[i][j]);
			return maxValue;
		}

		int maxIndexAsOneHot() {
			double maxValue = _item[0][0]; int maxIndex = 0;
			for (int j = 0; j < _col; j++) {
				maxValue = std::max(maxValue, _item[0][j]);
				if (maxValue == _item[0][j])
					maxIndex = j;
			}
			return maxIndex;
		}
#pragma endregion

#pragma region Static Mathematical Calculation

		// Returns negative of each elements
		static Matrix<T>* neg(const Matrix<T> * original) {
			Matrix<T>* negMat = new Matrix<T>(original->_row, original->_col);

			for (int i = 0; i < original->_row; i++)
				for (int j = 0; j < original->_col; j++)
					negMat->_item[i][j] = -(original->_item[i][j]);

			return negMat;
		}
#pragma endregion
	};
}