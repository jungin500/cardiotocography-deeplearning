#pragma once

#include <iostream>
#include <iterator>
#include <Vector>
#include <ctime>

/*
	DataFactory & TrainData
	Kangwon National University
	201513402 안정인 (jungin500@kangwon.ac.kr)
*/

namespace Cardiotocography {

	using namespace std;

	// TrainData
	// 입력 데이터셋의 데이터 하나하나를 나타낸다.
	class TrainData {
	public:
		Matrix<double>* data;
		Matrix<double>* correct_one_hot;

		// 주어지는 데이터셋은 21 크기이며, 총 레이블의 크기는 3이다(1~3).
		/// Constructor: length(dataset) == 21, total_set_of(label) == 3
		TrainData(double* dataset, int label) {
			double** dataset_dynamic = new double* [1]{ dataset };
			double** label_dynamic = new double* [1]{ new double[3] {0.0, 0.0, 0.0} };

			label_dynamic[0][label - 1] = 1.0;

			data = new Matrix<double>(1, 21, dataset_dynamic);
			correct_one_hot = new Matrix<double>(1, 3, label_dynamic);
		}
	};

	// DataFactory
	// 모든 데이터셋을 가지고 Train/Test 데이터로 분류하는 역할을 하며
	// 각 Train/Test 데이터를 가져올 수 있는 Interface를 제공한다.
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

		// 데이터를 데이터셋에다 넣는다.
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

		// 데이터셋을 섞는다.
		// 데이터셋을 섞었을 때 train_ratio로 주어진 계수(0.0~1.0)만큼의 비율로
		// Train 셋을, (1-train_ratio)만큼의 비율로 Test 셋을 결정한다.
		// 결정되는 각 셋은 무작위이다.
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