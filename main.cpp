#include "math.hpp"
#include "csv.hpp"
#include "matrix.hpp"
#include "SimpleFNN.hpp"
#include "DataFactory.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <assert.h>

using namespace std;
using namespace Cardiography;

CSVReader reader("dataset/cardiography-dataset.csv");
DataFactory datafactory;
SimpleFNN *network;

int main() {
	// Read whole data
	int label;
	double dataset[21];
	while (reader.nextLine()) {
		for (int i = 0; i < 21; i++)
			dataset[i] = reader.next<double>();
		label = reader.next<int>();

		TrainData data(dataset, label);
		datafactory.add(data);
	}

	// Randomize data
	datafactory.randomizeDataset(0.8);

	// Create network and Feed-Forward data
	network = new SimpleFNN(21, new int[1]{ 100 }, 1, 3);

	for (int i = 0; i < datafactory.testSize(); i++)
	{
		if (!datafactory.hasNextTest())
			break;

		TrainData train = datafactory.nextTest();
		double *data[1] = { train.data };
		double *correct[1] = { new double[3] { .0, .0, .0 } };
		correct[0][train.label - 1] = 1.0;

		Matrix<double> *correct_one_hot = new Matrix<double>(1, 3, correct);
		Matrix<double> *test_data = new Matrix<double>(1, 21, data);

		Matrix<double> forward0 = network->forward(test_data, 0);
		forward0.each(sigmoid);

		Matrix<double> result = network->forward(&forward0, 1);
		Matrix<double> sqError = result.squaredError(correct_one_hot);

		
	}

}