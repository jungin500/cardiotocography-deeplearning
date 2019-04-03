#include "matrix.hpp"

#include <iostream>
#include <vector>

namespace Cardiography {

	using namespace std;

	class SimpleFNN {
	private:
		int inputLayerDims;
		int outputLayerDims;
		vector<Matrix<double> *> weightMatrix;

	public:
		SimpleFNN(const int &input_layer, const int *hidden_layer,
			const int &hidden_layer_size, const int &output_layer)
			: inputLayerDims(input_layer), outputLayerDims(output_layer) {

			weightMatrix.push_back(new Matrix<double>(inputLayerDims, hidden_layer[0]));

			for (int i = 1; i < hidden_layer_size; i++)
				weightMatrix.push_back(new Matrix<double>(hidden_layer[i - 1], hidden_layer[i]));

			weightMatrix.push_back(new Matrix<double>(hidden_layer[hidden_layer_size - 1], outputLayerDims));

			print();
		}

		Matrix<double> forwardAll(const Matrix<double> *data) {
			Matrix<double> result(*data);

			for (int i = 0; i < weightMatrix.size(); i++) {
				Matrix<double> weights = *weightMatrix.at(i);
				result = result * weights; // repeated `size()`th
			}

			return result;
		}

		Matrix<double> forward(const Matrix<double> *data, const int &index) {
			Matrix<double> original(*data);
			return original * (*weightMatrix.at(index));
		}

		void print() {
			for (int i = 0; i < weightMatrix.size(); i++)
				cout << weightMatrix.at(i)->size().rows << "x" << weightMatrix.at(i)->size().cols << "\t";
			cout << "Weight Matrix with Layer(" << weightMatrix.size() << ")" << endl;
		}

	};
}