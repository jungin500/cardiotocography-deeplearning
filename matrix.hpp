#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

/*
	DataFactory & TrainData
	Kangwon National University
	201513402 안정인 (jungin500@kangwon.ac.kr)
*/

namespace Cardiotocography {

	using namespace std;

	struct Size {
		int rows;
		int cols;
	};

	// Matrix
	// 각종 행렬 연산을 할 수 있는 클래스이다.
	template<typename T = double>
	class Matrix {
	private:
		T** _item;
		int _row;
		int _col;

	public:

		// Constructor
		// 열과 행을 받으면, 해당 크기를 생성하고 랜덤한 수로 채운다.
		// 랜덤한 수는 -0.025 ~ 0.025 까지의 범위를 가진다. (ppt자료 참고)
		Matrix(int row, int col) : _row(row), _col(col) {
			_item = new T * [row];
			for (int i = 0; i < row; i++)
				_item[i] = new T[col];
			fillRandomNumber();
		}

		// Constructor
		// 열과 행, 그리고 데이터를 받으면
		// 해당 크기의 데이터 공간을 생성하고 데이터는 복사한다.
		Matrix(int row, int col, T * data[]) : Matrix(row, col) {
			// assert(sizeof(data) == sizeof(T) * row);
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++)
					_item[i][j] = data[i][j];
		}

		// Destructor
		// Matrix의 context가 사라졌을 때는 가지고 있는 배열을 삭제하고
		// 메모리를 release할 수 있도록 한다.
		~Matrix() {
			for (int i = 0; i < _row; i++)
				delete[] _item[i];
			delete[] _item;
		}

		// Copy Constructor
		// 다른 Matrix를 복사할 때, 기존의 값들의 reference가 아닌
		// 실제 값들을 생성해서 가지고 있는다. Destructor와 같이
		// 사용되어야 하기때문에 반드시 재정의한다.
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

		// 무작위 수를 채워준다.
		// 빈 내용의 Matrix를 생성하게 되면, 이를 Weight Matrix로
		// 가정하고 -0.025부터 0.025까지의 수 중 하나를 무작위로
		// 모든 배열 내용에 채워준다.
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

		// 해당하는 Row와 Column의 값을 반환한다.
		T at(int row, int col) {
			if (row < 0 || this->_row <= row || col < 0 || this->_col <= col)
				throw new exception("ArrayOutOfBoundsException");
			return _item[row][col];
		}

		// 모든 Element를 함수를 수행할 수 있다.
		// 인자로 double <function>(double original) 형 함수를 받는다.
		void each(double(*fp)(double)) {
			for (int i = 0; i < _row; i++)
				for (int j = 0; j < _col; j++)
					_item[i][j] = fp(_item[i][j]);
		}

		// 해당하는 Row와 Column의 Element를 인자 data로 채운다.
		void set(int row, int col, T data) {
			if (row < 0 || this->_row <= row || col < 0 || this->_col <= col)
				throw new exception("ArrayOutOfBoundsException");
			_item[row][col] = data;
		}

		// 현재 Matrix의 크기를 나타내는 구조체를 반환한다.
		Size size() {
			Size sz;
			sz.rows = _row;
			sz.cols = _col;
			return sz;
		}

		// operator*를 재정의(override)한다.
		// 해당 연산의 내용은 행렬 곱(Matrix Multiplication)이다.
		Matrix<T> operator* (const Matrix<T> & another) {
			if (another._row != _col)
				throw new std::exception("InvalidMultiplicationSizeException");

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

		// 해당하는 Matrix를 표준 출력으로 출력한다.
		static void print(const Matrix<T> & mat) {
			for (int j = 0; j < mat._row; j++) {
				for (int i = 0; i < mat._col; i++) {
					cout << mat._item[j][i];
					if (i != mat._col - 1)
						cout << ",\t";
				}
				cout << "\n";
			}
		}

		// 현재 Matrix를 출력한다.
		void print() {
			print(*this);
		}

#pragma region Mathematical Calculation
		// MSE (Mean Squared Error) 중에서, Squared Error에 해당하는 부분만 수정한다.
		Matrix<T>* squaredError(const Matrix<T> * correct_one_hot) {
			// assert(correct_one_hot->row == 1 && row == 1 && correct_one_hot->col == col);

			Matrix<T>* error = new Matrix<T>(1, _col);
			for (int i = 0; i < _col; i++)
				error.item[0][i] = pow(correct_one_hot->_item[0][i] - _item[0][i], 2);

			return error;
		}

		// 오차를 구해 반환한다.
		Matrix<T>* differenceError(const Matrix<T> * correct_one_hot) {
			Matrix<T>* error = new Matrix<T>(1, _col);
			for (int i = 0; i < _col; i++)
				error->_item[0][i] = correct_one_hot->_item[0][i] - _item[0][i];

			return error;
		}

		// 전치(Transpose)
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

		// 행렬의 모든 원소 중 최대값을 구한다.
		double max() {
			double maxValue = _item[0][0];
			for (int i = 0; i < _row; i++)
				for (int j = 0; j < _col; j++)
					maxValue = std::max(maxValue, _item[i][j]);
			return maxValue;
		}

		// 최대 원소의 인덱스를 반환한다.
		/// Suppose row == 1
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