#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace CSVParser {

	using namespace std;

	class Row {
	public:
		string *data;
		int size;

		Row() { }
		Row(string *data, int size) : data(data), size(size) { }
		
		string operator[] (int idx) {
			if (size <= idx || idx < 0)
				return "";
			return data[idx];
		}
	};

	class DataReader {

	private:
		Row titleRow;
		ifstream dataStream;

	public:
		DataReader(string filePath) {
			dataStream = ifstream(filePath.data());
		}

		Row readNextRow() {
			if (!dataStream.is_open())
				throw new exception("Row read error: Data stream is closed!");

			string line, token;
			vector<string> data;

			if (!getline(dataStream, line))
				throw new exception("Row read error: EOF");

			size_t tokPos;
			while ((tokPos = line.find(',')) != string::npos) {
				token = line.substr(0, tokPos);
				data.push_back(token);
				line.erase(0, tokPos + 1);
			}
		}
	};
}

/*
int main() {
	using namespace CSVParser;
	using namespace std;

	DataReader reader("dataset/CTG.csv");
	int tmp;

	while (1) {
		Row next = reader.readNextRow();
		for (int i = 0; i < next.size; i++)
			cout << next[i] << endl;
		cin >> tmp;
	}
}
*/