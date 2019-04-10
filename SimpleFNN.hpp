#pragma once

#include "math.hpp"
#include "matrix.hpp"

#include <iostream>
#include <vector>

#define LEARNING_RATE 12

namespace Cardiography {

	using namespace std;

	class SimpleFNN {
	private:
		int inputLayerDims;
		int outputLayerDims;
		vector<Matrix<double>*> weightMatrix;

	public:
		SimpleFNN(const int& input_layer, const int* hidden_layer,
			const int& hidden_layer_size, const int& output_layer)
			: inputLayerDims(input_layer), outputLayerDims(output_layer) {

			weightMatrix.push_back(new Matrix<double>(inputLayerDims, hidden_layer[0]));

			for (int i = 1; i < hidden_layer_size; i++)
				weightMatrix.push_back(new Matrix<double>(hidden_layer[i - 1], hidden_layer[i]));

			weightMatrix.push_back(new Matrix<double>(hidden_layer[hidden_layer_size - 1], outputLayerDims));

			print();
		}

		Matrix<double> forwardAll(const Matrix<double> * data) {
			Matrix<double> result(*data);

			for (int i = 0; i < weightMatrix.size(); i++) {
				Matrix<double> weights = *weightMatrix.at(i);
				result = result * weights; // repeated `size()`th
			}

			return result;
		}

		Matrix<double> forward(const Matrix<double> * data, const int& index) {
			Matrix<double> original(*data);
			return original * (*weightMatrix.at(index));
		}

		void backward(const Matrix<double> * sqError) {
			// assert(sqError->size().rows == 1 && sqError->size().cols == 3);

			// �̺е� Sigmoid(dSigmoid)�� ������ Error result
			Matrix<double> dsgSqError(*sqError);
			dsgSqError.each(dsigmoid);

			// ���� ���� ������ �߰� ���̾�
			vector<Matrix<double>*> backpropLayers;
			for (int i = 0; i < weightMatrix.size(); i++)
				backpropLayers.push_back(new Matrix<double>(1, weightMatrix.at(i)->size().cols));

			// ���������� Backpropagation
			for (int i = weightMatrix.size() - 1; i >= 0; i--) {
				Matrix<double>* currentWeight = weightMatrix.at(i);

				Matrix<double>* currentBackpropLayer = backpropLayers.at(backpropLayers.size() - 1); // ������ ���̾ �����´�. ����� �ٷ� ��.
				for (int k = 0; k < 3; k++)
					for (int j = 0; j < currentWeight->size().cols; j++) {
						double w_sum = .0;

						for (int i = 0; i < currentWeight->size().rows; i++)
							w_sum += currentWeight->at(i, k); // 0�� ���� y_[1]�� Backpropagation�̱� ����!

						double divide = currentWeight->at(j, k) / w_sum; // 0,0 => 0 x_[1]�� Backpropagation�̸� y_[1]���̱� ����
						currentBackpropLayer->set(k, 0, divide * LEARNING_RATE);
					}

				cout << "BackpropLayer" << endl;
				currentBackpropLayer->print();
			}
		}

		void print() {
			for (int i = 0; i < weightMatrix.size(); i++)
				cout << weightMatrix.at(i)->size().rows << "x" << weightMatrix.at(i)->size().cols << "\t";
			cout << "Weight Matrix with Layer(" << weightMatrix.size() << ")" << endl;
		}

	};
}