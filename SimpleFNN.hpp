#pragma once

#include "math.hpp"
#include "matrix.hpp"
#include "DataFactory.hpp"

#include <iostream>
#include <vector>

constexpr auto LEARNING_RATE = 12;

namespace Cardiotocography {

	using namespace std;

	class SimpleFNN {
	private:
		int inputLayerDims;
		int outputLayerDims;
		vector<Matrix<double>*> weightMatrixes;
		vector<Matrix<double>*> hiddenInstanceMatrixes;

#pragma region Static Mathmatical Functions
		// Get backpropagation weight update value(delta)
		// error: pre-calculated -(t_k - o_k)
		static double getBPWeightUpdateDelta(const int& upstreamIndex, const int& downstreamIndex, const double& error,
			Matrix<double>* weightMatrix, Matrix<double>* upstreamHidden, Matrix<double>* downstreamHidden) {
			double sum = .0;
			for (int i = 0; i < weightMatrix->size().rows; i++) {
				double weightMatrixValue = weightMatrix->at(i, downstreamIndex);
				double upstreamHiddenMatrixValue = upstreamHidden->at(0, i);
				sum += weightMatrixValue * upstreamHiddenMatrixValue;
			}

			double upstreamHiddenValue = upstreamHidden->at(0, upstreamIndex);
			return error * dsigmoid(sum) * upstreamHiddenValue;
		}

		// Get next upstream node's value
		static double getBPNextNodeValue(const int& upstreamIndex, Matrix<double> * weightMatrix, Matrix<double> * downstreamHidden) {
			double nextNodeVal = .0;
			int weightMatrixColumnCount = weightMatrix->size().cols;
			for (int j = 0; j < weightMatrixColumnCount; j++) {
				double currWeightVal = .0;
				for (int i = 0; i < weightMatrixColumnCount; i++)
					currWeightVal += weightMatrix->at(upstreamIndex, i);
				currWeightVal = weightMatrix->at(upstreamIndex, j) / currWeightVal * downstreamHidden->at(0, j);
				nextNodeVal += currWeightVal;
			}
			return nextNodeVal;
		}

		// Perform backpropagation weight update
		static void updateBPWeight(const int& row, const int& col, const double& weight_update, const double& learning_rate, Matrix<double> * weightMatrix) {
			weightMatrix->set(row, col, weightMatrix->at(row, col) - learning_rate * weight_update);
		}
#pragma endregion

	public:
		SimpleFNN(const int& input_layer, const int* hidden_layer,
			const int& hidden_layer_size, const int& output_layer)
			: inputLayerDims(input_layer), outputLayerDims(output_layer) {

			weightMatrixes.push_back(new Matrix<double>(inputLayerDims, hidden_layer[0]));

			for (int i = 1; i < hidden_layer_size; i++)
				weightMatrixes.push_back(new Matrix<double>(hidden_layer[i - 1], hidden_layer[i]));

			weightMatrixes.push_back(new Matrix<double>(hidden_layer[hidden_layer_size - 1], outputLayerDims));

			print();
			cout << "SimpleFNN Initialized..." << endl << endl;
		}

		// test_data를 forward해서 Result를 반환한다.
		Matrix<double>* forward(const TrainData* test_data) {
			Matrix<double> data(*(test_data->data)), * result, * hidden = nullptr;
			
			// Remove all instance matrixes before start
			while (!hiddenInstanceMatrixes.empty()) {
				Matrix<double>* hidden = hiddenInstanceMatrixes.at(hiddenInstanceMatrixes.size() - 1);
				hiddenInstanceMatrixes.pop_back();
				delete hidden;
			}

			for (int i = 0; i < weightMatrixes.size(); i++) {
				// hiddenMatrix의 크기는 중간 weightMatrix의 Column 수로 예측한다

				if (i == 0)
					hidden = new Matrix<double>(data * (*weightMatrixes.at(i)));
				else
					hidden = new Matrix<double>((*hiddenInstanceMatrixes.at(i - 1)) * (*weightMatrixes.at(i)));

				// Sigmoid for each function (including last one == output)
				hidden->each(sigmoid);

				// 마지막 hiddenMatrix == 출력 Matrix
				if (i != weightMatrixes.size() - 1)
					// for Backpropagation calculation
					hiddenInstanceMatrixes.push_back(hidden);
			}

			if (hidden == nullptr)
				throw new exception("HiddenMatrixNotPresentException");
			return hidden;
		}

		// Returns target function result
		double backward(const TrainData * train_data) {
			Matrix<double>* result = forward(train_data);
			Matrix<double>* one_hot_label = train_data->correct_one_hot;

			// Error value => difference 
			Matrix<double>* error = Matrix<double>::neg(result->differenceError(train_data->correct_one_hot));

			// (Error) * (Sigmoid`) * (Previous Output)
			// from End to 2nd hidden layers
			Matrix<double>* hiddenInstanceMatrixZero = hiddenInstanceMatrixes.at(0);

			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < 3; j++) {
					double delta = getBPWeightUpdateDelta(i, j, error->at(0, j), weightMatrixes.at(1), hiddenInstanceMatrixZero, error); // last matrix is 100*3
					updateBPWeight(i, j, delta, LEARNING_RATE, weightMatrixes.at(1));
				}

				double bpNodeVal = getBPNextNodeValue(i, weightMatrixes.at(1), result);
				hiddenInstanceMatrixes.at(0)->set(0, i, bpNodeVal); // last hidden Matrix is 0
			}

			return result->mse(one_hot_label);
		}

		void print() {
			for (int i = 0; i < weightMatrixes.size(); i++)
				cout << weightMatrixes.at(i)->size().rows << "x" << weightMatrixes.at(i)->size().cols << "\t";
			cout << "Weight Matrix with Layer(" << weightMatrixes.size() << ")" << endl;
		}

	};
}