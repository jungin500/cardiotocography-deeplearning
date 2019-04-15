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

CSVReader __reader("dataset/cardiography-dataset.csv");
DataFactory __datafactory;

SimpleFNN* __networkBig;
SimpleFNN* __networkNarrow;
SimpleFNN* __networkSmall;

int main() {
	// Read whole data
	int label;
	double dataset[21];
	while (__reader.nextLine()) {
		for (int i = 0; i < 21; i++)
			dataset[i] = __reader.next<double>();
		label = __reader.next<int>();

		TrainData data(dataset, label);
		__datafactory.add(data);
	}

	// Randomize data
	__datafactory.randomizeDataset(0.8);

	// Create network and Backpropagate data
	int hiddenNeuronCnt1 = 100, hiddenNeuronCnt2 = 20;
	__networkBig = new SimpleFNN(21, new int[1]{ hiddenNeuronCnt1 }, 1, 3, 12.0);
	__networkNarrow = new SimpleFNN(21, new int[1]{ hiddenNeuronCnt1 }, 1, 3, 3.0);
	__networkSmall = new SimpleFNN(21, new int[1]{ hiddenNeuronCnt2 }, 1, 3, 12.0);

	// one epoch
	double mseEpochBig = .0, mseEpochNarrow = .0, mseEpochSmall = .0;
	__datafactory.resetTestPtr();

	for (int i = 0; i < __datafactory.testSize(); i++)
	{
		if (!__datafactory.hasNextTest())
			break;
		TrainData test = __datafactory.nextTest();

		mseEpochBig += __networkBig->backward(&test);
		mseEpochNarrow += __networkNarrow->backward(&test);
		mseEpochSmall += __networkSmall->backward(&test);
	}
	printf_s("\t\t__networkBig(%d:%.1f)\t\t__networkNarrow(%d:%.1f)\t__networkNarrow(%d:%.1f)\n", hiddenNeuronCnt1, 12.0, hiddenNeuronCnt1, 3.0, hiddenNeuronCnt2, 12.0);
	printf_s("BeginTest MSE:\t%.8f\t\t\t%.8f\t\t\t%.8f\n\n", mseEpochBig, mseEpochNarrow, mseEpochSmall);

	// 50 epoch
	for (int j = 0; j < 50; j++) {

		printf_s("[Epoch %02d]\t__networkBig(%d:%.1f)\t\t__networkNarrow(%d:%.1f)\t__networkNarrow(%d:%.1f)\n", j, hiddenNeuronCnt1, 12.0, hiddenNeuronCnt1, 3.0, hiddenNeuronCnt2, 12.0);
		// Train each
		mseEpochBig = .0, mseEpochNarrow = .0, mseEpochSmall = .0;
		__datafactory.resetTrainPtr();

		for (int i = 0; i < __datafactory.trainSize(); i++)
		{
			if (!__datafactory.hasNextTrain())
				break;
			TrainData train = __datafactory.nextTrain();

			mseEpochBig += __networkBig->backward(&train);
			mseEpochNarrow += __networkBig->backward(&train);
			mseEpochSmall += __networkBig->backward(&train);
		}
		printf_s("Train MSE:\t%.8f\t\t\t%.8f\t\t\t%.8f\n", mseEpochBig, mseEpochNarrow, mseEpochSmall);

		// Test each
		mseEpochBig = .0, mseEpochNarrow = .0, mseEpochSmall = .0;
		__datafactory.resetTestPtr();

		for (int i = 0; i < __datafactory.testSize(); i++)
		{
			if (!__datafactory.hasNextTest())
				break;
			TrainData test = __datafactory.nextTest();

			mseEpochBig += __networkBig->backward(&test);
			mseEpochNarrow += __networkNarrow->backward(&test);
			mseEpochSmall += __networkSmall->backward(&test);
		}
		printf_s("Test MSE:\t%.8f\t\t\t%.8f\t\t\t%.8f\n\n", mseEpochBig, mseEpochNarrow, mseEpochSmall);
	}
}