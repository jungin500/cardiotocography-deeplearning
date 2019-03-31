#include "csv.hpp"
#include "matrix.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <assert.h>

namespace Cardiography {

	double sigmoid(double input) {
		return 1 / (1 + exp(-input));
	}

	template <typename T = double *>
	class TrainData {
	public:
		int dataType;
		T data;
		int label;

		TrainData(T data, int label) : data(data), label(label) {}
	};

	class DataFactory {
	private:

	public:
		DataFactory() {}

		void add(const TrainData<double *> &data) {
			fulldata.push_back(data);
		}

#pragma region Train/Test Dataset Getter

		bool hasNextTrain() {
			return trainidx_ptr < train_index.size();
		}

		bool hasNextTest() {
			return testidx_ptr < test_index.size();
		}

		TrainData<double *> nextTrain() {
			if (!hasNextTrain())
				throw new exception("EndOfTrainDatasetException");
			return fulldata.at(train_index.at(trainidx_ptr++));
		}

		TrainData<double *> nextTest() {
			if (!hasNextTest())
				throw new exception("EndOfTestDatasetException");
			return fulldata.at(test_index.at(testidx_ptr++));
		}

#pragma endregion
		vector<TrainData<double *>> fulldata;
		vector<int> train_index, test_index;
		int trainidx_ptr, testidx_ptr;

		/// train_ratio between 0.0 and 1.0
		void randomizeDataset(const double &train_ratio) {
			if (!(0.0 <= train_ratio && train_ratio <= 1.0))
				throw new exception("InvalidTrainRatioException");

			int full_size = fulldata.size();
			int train_size = (int)fulldata.size() * train_ratio;
			int test_size = fulldata.size() - train_size;

			srand((unsigned int)time(NULL));

			while (train_index.size() < train_size) {
				int pick = rand() % full_size;
				vector<int>::iterator it = find(train_index.begin(), train_index.end(), pick);
				if (it != train_index.end())
					continue;
				train_index.push_back(pick);
			}

			while (test_index.size() < test_size) {
				int pick = rand() % full_size;
				vector<int>::iterator train_it = find(train_index.begin(), train_index.end(), pick);
				vector<int>::iterator test_it = find(test_index.begin(), test_index.end(), pick);
				if (train_it != train_index.end() || test_it != test_index.end())
					continue;
				test_index.push_back(pick);
			}

			trainidx_ptr = testidx_ptr = 0;

			cout << "Randomize complete, train size=" << train_index.size()
				<< ", test size=" << test_index.size() << endl;
		}
	};

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

using namespace std;
using namespace Cardiography;

CSVReader reader("dataset/cardiography-dataset.csv");
DataFactory datafactory;
SimpleFNN *network;

int main() {
	// Read whole data
	while (reader.nextLine()) {
		double dataset[21];
		int label;

		for (int i = 0; i < 21; i++)
			dataset[i] = reader.next<double>();
		label = reader.next<int>();

		TrainData<double *> data(dataset, label);
		datafactory.add(data);
	}

	// Randomize data
	datafactory.randomizeDataset(0.8);

	// Create network and Feed-Forward data
	network = new SimpleFNN(21, new int[1]{ 10 }, 1, 3);

	double *data[1] = { datafactory.nextTest().data };

	Matrix<double> *test_data = new Matrix<double>(1, 21, data);
	cout << "Test Data:" << endl;
	test_data->print();
	
	Matrix<double> forward0 = network->forward(test_data, 0);
	cout << "Forward0:" << endl;
	forward0.print();
	forward0.each(sigmoid);

	Matrix<double> forward1 = network->forward(&forward0, 1);
	forward1.each(sigmoid);
	cout << "Forward1:" << endl;
	forward1.print();
}