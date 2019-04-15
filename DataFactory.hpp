#pragma once

#include <iostream>
#include <iterator>
#include <Vector>
#include <ctime>

namespace Cardiotocography {

	using namespace std;

	class TrainData {
	public:
		Matrix<double> *data;
		Matrix<double> *correct_one_hot;

		// length(dataset) == 21, total_set_of(label) == 3
		TrainData(double* dataset, int label) {
			double** dataset_dynamic = new double* [1]{ dataset };
			double** label_dynamic = new double* [1]{ new double[3] {0.0, 0.0, 0.0} };
			
			data = new Matrix<double>(1, 21, dataset_dynamic);
			correct_one_hot = new Matrix<double>(1, 3, label_dynamic);
		}
	};

	class DataFactory {
	private:
		vector<TrainData> fulldata;
		vector<int> train_index, test_index;
		int trainidx_ptr, testidx_ptr;

	public:
		DataFactory() {
			trainidx_ptr = 0;
			testidx_ptr = 0;
		}

		void add(const TrainData& data) {
			fulldata.push_back(data);
		}

#pragma region Train/Test Dataset Getter

		bool hasNextTrain() {
			return trainidx_ptr < train_index.size();
		}

		bool hasNextTest() {
			return testidx_ptr < test_index.size();
		}

		int trainSize() {
			return train_index.size();
		}

		int testSize() {
			return test_index.size();
		}

		void resetTrainPtr() {
			trainidx_ptr = 0;
		}

		void resetTestPtr() {
			testidx_ptr = 0;
		}

		TrainData nextTrain() {
			if (!hasNextTrain())
				throw new exception("EndOfTrainDatasetException");
			return fulldata.at(train_index.at(trainidx_ptr++));
		}

		TrainData nextTest() {
			if (!hasNextTest())
				throw new exception("EndOfTestDatasetException");
			return fulldata.at(test_index.at(testidx_ptr++));
		}

#pragma endregion

		/// train_ratio between 0.0 and 1.0
		void randomizeDataset(const double& train_ratio) {
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
}