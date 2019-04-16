#pragma once

#include "math.hpp"
#include "matrix.hpp"
#include "DataFactory.hpp"

#include <iostream>
#include <vector>

/*
	DataFactory & TrainData
	Kangwon National University
	201513402 안정인 (jungin500@kangwon.ac.kr)
*/

namespace Cardiotocography {

	using namespace std;

	// SimpleFNN
	// 
	class SimpleFNN {
	private:
		int _inputLayerDims;
		int _outputLayerDims;
		vector<Matrix<double>*> _weightMatrixes;
		vector<Matrix<double>*> _hiddenInstanceMatrixes;
		double _learning_rate;

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
			return error * dsigmoid(sum)* upstreamHiddenValue;
		}

		// Get next upstream node's value
		static double getBPNextNodeValue(const int& upstreamIndex,
			Matrix<double>* weightMatrix, Matrix<double>* downstreamHidden) {
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
		static void updateBPWeight(const int& row, const int& col,
			const double& weight_update, const double& learning_rate, Matrix<double> * weightMatrix) {
			weightMatrix->set(row, col, weightMatrix->at(row, col) - learning_rate * weight_update);
		}
#pragma endregion

	public:
		SimpleFNN(const int& input_layer, const int* hidden_layer,
			const int& hidden_layer_size, const int& output_layer, const double learning_rate)
			: _inputLayerDims(input_layer), _outputLayerDims(output_layer) {

			// set as global variable
			_learning_rate = learning_rate;
			cout << "Learning rate: " << _learning_rate << endl;

			_weightMatrixes.push_back(new Matrix<double>(_inputLayerDims, hidden_layer[0]));

			for (int i = 1; i < hidden_layer_size; i++)
				_weightMatrixes.push_back(new Matrix<double>(hidden_layer[i - 1], hidden_layer[i]));

			_weightMatrixes.push_back(new Matrix<double>(hidden_layer[hidden_layer_size - 1], _outputLayerDims));

			print();
			std::cout << "SimpleFNN Initialized..." << endl << endl;
		}

		// test_data를 forward해서 Result를 반환한다.
		Matrix<double> * forward(const TrainData * test_data) {
			Matrix<double> data(*(test_data->data)), * result, * hidden = nullptr;

			// Remove all instance matrixes before start
			while (!_hiddenInstanceMatrixes.empty()) {
				Matrix<double>* hidden = _hiddenInstanceMatrixes.at(_hiddenInstanceMatrixes.size() - 1);
				_hiddenInstanceMatrixes.pop_back();
				delete hidden;
			}

			for (int i = 0; i < _weightMatrixes.size(); i++) {
				// hiddenMatrix의 크기는 중간 weightMatrix의 Column 수로 예측한다

				if (i == 0)
					hidden = new Matrix<double>(data * (*_weightMatrixes.at(i)));
				else
					hidden = new Matrix<double>((*_hiddenInstanceMatrixes.at(i - 1)) * (*_weightMatrixes.at(i)));

				// Sigmoid for each function (including last one == output)
				hidden->each(sigmoid);

				// 마지막 hiddenMatrix == 출력 Matrix
				if (i != _weightMatrixes.size() - 1)
					// for Backpropagation calculation
					_hiddenInstanceMatrixes.push_back(hidden);
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
			Matrix<double>* hiddenInstanceMatrixZero = _hiddenInstanceMatrixes.at(0);

			for (int i = 0; i < _weightMatrixes.at(0)->size().cols; i++) {
				for (int j = 0; j < 3; j++) {
					double delta = getBPWeightUpdateDelta(i, j, error->at(0, j),
						_weightMatrixes.at(1), hiddenInstanceMatrixZero, error); // last matrix is 100*3
					updateBPWeight(i, j, delta, _learning_rate, _weightMatrixes.at(1));
				}

				double bpNodeVal = getBPNextNodeValue(i, _weightMatrixes.at(1), result);
				_hiddenInstanceMatrixes.at(0)->set(0, i, bpNodeVal); // last hidden Matrix is 0
			}

			return result->mse(one_hot_label);
		}

		void print() {
			for (int i = 0; i < _weightMatrixes.size(); i++)
				std::cout << _weightMatrixes.at(i)->size().rows << "x" << _weightMatrixes.at(i)->size().cols << "\t";
			std::cout << "Weight Matrix with Layer(" << _weightMatrixes.size() << ")" << endl;
		}

	};
}