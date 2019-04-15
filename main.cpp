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
using namespace Cardiotocography;

CSVReader reader("dataset/cardiography-dataset.csv");
DataFactory datafactory;
SimpleFNN* network;

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

	// Create network and Backpropagate data
	int hiddenLayerNeruonCount = 100;
	network = new SimpleFNN(21, new int[1]{ hiddenLayerNeruonCount }, 1, 3);

	// one epoch
	double mseEpoch = .0;
	datafactory.resetTestPtr();

	for (int i = 0; i < datafactory.testSize(); i++)
	{
		if (!datafactory.hasNextTest())
			break;
		TrainData test = datafactory.nextTest();
		mseEpoch += network->backward(&test);
	}
	printf_s("BeginTest MSE:\t\t%.8f\n\n", mseEpoch);

	// Train
	for (int j = 0; j < 100; j++) {
		mseEpoch = .0;
		datafactory.resetTrainPtr();

		for (int i = 0; i < datafactory.trainSize(); i++)
		{
			if (!datafactory.hasNextTrain())
				break;
			TrainData train = datafactory.nextTrain();

			/*
			// 일단 한번 돌려본다.
			Matrix<double>* error = network->forward(&train);
			double mse1 = error->mse(train.correct_one_hot);

			cout << "First try MSE: " << mse1 << endl;
			*/

			// backward를 한번 진행해본다.
			mseEpoch += network->backward(&train);
		}

		printf_s("TrainEpoch %03d MSE:\t%.8f\n", j + 1, mseEpoch);

		// Test each
		mseEpoch = .0;
		datafactory.resetTestPtr();

		for (int i = 0; i < datafactory.testSize(); i++)
		{
			if (!datafactory.hasNextTest())
				break;
			TrainData test = datafactory.nextTest();
			mseEpoch += network->backward(&test);
		}
		printf_s("TestEpoch %03d MSE:\t%.8f\n\n", j + 1, mseEpoch);
	}
}