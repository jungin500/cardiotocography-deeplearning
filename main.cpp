#include "math.hpp"
#include "csv.hpp"
#include "matrix.hpp"
#include "SimpleFNN.hpp"
#include "DataFactory.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <assert.h>

using namespace std;
using namespace Cardiotocography;

const string __outputFilename = "dataset/backpropagation-report-unitcount.csv";

CSVReader __reader("dataset/cardiography-dataset.csv");
CSVWriter __writer;
DataFactory __datafactory;

SimpleFNN* __testNetwork1;
SimpleFNN* __testNetwork2;
SimpleFNN* __networkSmall;

int main() {
	__writer.open(__outputFilename);

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
	int hiddenNeuronCnt1 = 80, hiddenNeuronCnt2 = 10;

	__testNetwork1 = new SimpleFNN(21, new int[1]{ hiddenNeuronCnt1 }, 1, 3, 0.2);
	__testNetwork2 = new SimpleFNN(21, new int[1]{ hiddenNeuronCnt2 }, 1, 3, 0.2);

	// one epoch
	double mseTestNetwork1 = .0, mseTestNetwork2 = .0;
	__datafactory.resetTestPtr();

	for (int i = 0; i < __datafactory.testSize(); i++)
	{
		if (!__datafactory.hasNextTest())
			break;
		TrainData test = __datafactory.nextTest();

		mseTestNetwork1 += __testNetwork1->backward(&test);
		mseTestNetwork2 += __testNetwork2->backward(&test);
	}

	string writeData = "epoch,mseTestNetwork1,mseTestNetwork2\n";
	__writer.write(writeData, writeData.size());

	cout << writeData;

	// 50 epoch
	for (int j = 0; j < 200; j++) {

		// Train each
		mseTestNetwork1 = .0, mseTestNetwork2 = .0;
		__datafactory.resetTrainPtr();

		for (int i = 0; i < __datafactory.trainSize(); i++)
		{
			if (!__datafactory.hasNextTrain())
				break;
			TrainData train = __datafactory.nextTrain();

			mseTestNetwork1 += __testNetwork1->backward(&train);
			mseTestNetwork2 += __testNetwork1->backward(&train);
		}

		// Test each
		mseTestNetwork1 = .0, mseTestNetwork2 = .0;
		__datafactory.resetTestPtr();

		for (int i = 0; i < __datafactory.testSize(); i++)
		{
			if (!__datafactory.hasNextTest())
				break;
			TrainData test = __datafactory.nextTest();

			mseTestNetwork1 += __testNetwork1->backward(&test);
			mseTestNetwork2 += __testNetwork2->backward(&test);
		}

		stringstream ss;
		ss << j + 1 << "," << mseTestNetwork1 << "," << mseTestNetwork2 << "," << endl;

		string writeStr = ss.str();
		__writer.write(writeStr, writeStr.size());

		cout << writeStr;
	}

	cout << "Wrote " << 200 << "Epochs into \"" << __outputFilename << "\"." << endl;

	return 0;
}