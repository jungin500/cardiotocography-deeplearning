#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

namespace Cardiography {
	class CSVReader {
	private:
		ifstream fin;
		int lineno;
		string *header;

	public:
		CSVReader(const string &filename) {
			open(filename);
			lineno = 0;
		}

		CSVReader() {
			lineno = -1;
		}
		
		void open(const string &filename) {
			if (fin.is_open())
				fin.close();
			fin.open(filename, ifstream::in);
		}

		void close() {
			if(fin.is_open())
				fin.close();
		}

		bool isOpen() {
			return fin.is_open();
		}

		string readLine() {
			if (!fin.eof()) {
				string line;
				getline(fin, line);
			}

			return line;
		}
	};
}