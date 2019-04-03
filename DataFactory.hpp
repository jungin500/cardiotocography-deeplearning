#pragma once

#include <iostream>
#include <iterator>
#include <Vector>
#include <ctime>

namespace Cardiography {

	using namespace std;

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
		vector<TrainData<double *>> fulldata;
		vector<int> train_index, test_index;
		int trainidx_ptr, testidx_ptr;

	public:
		DataFactory() {
			trainidx_ptr = 0;
			testidx_ptr = 0;
		}

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

		int trainSize() {
			return train_index.size();
		}

		int testSize() {
			return test_index.size();
		}

		TrainData<double *> nextTrain() {
			if (!hasNextTrain())
				throw new exception("EndOfTrainDatasetException");
			return fulldata.at(train_index.at(trainidx_ptr++));
		}

		TrainData<double *> nextTest() {
			if (!hasNextTest())
				throw new exception("EndOfTestDatasetException");

			cout << testidx_ptr << endl;
			return fulldata.at(test_index.at(testidx_ptr++));
		}

#pragma endregion

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
}