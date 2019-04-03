#include "csv.hpp"
#include "matrix.hpp"
#include "SimpleFNN.hpp"
#include "DataFactory.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <assert.h>

namespace Cardiography {
	double sigmoid(double input) {
		return 1 / (1 + exp(-input));
	}
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

	for(int i = 0; i < datafactory.testSize(); i++)

	double *data[1] = { datafactory.nextTest().data };

	Matrix<double> *test_data = new Matrix<double>(1, 21, data);
	cout << "Test Data:" << endl;
	test_data->print();
	
	Matrix<double> forward0 = network->forward(test_data, 0);
	cout << "Forward0:" << endl;
	forward0.print();
	forward0.each(sigmoid);

	Matrix<double> result = network->forward(&forward0, 1);
	result.each(sigmoid);
	cout << "Forward1:" << endl;
	result.print();
}