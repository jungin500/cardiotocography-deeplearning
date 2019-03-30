#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

namespace Cardiography {

	const string header_name[22] = { "LB", "AC", "FM", "UC",
		"DL", "DS", "DP", "ASTV", "MSTV", "ALTV", "MLTV", "Width",
		"Min", "Max", "Nmax", "Nzeros", "Mode", "Mean", "Median",
		"Variance", "Tendency", "NSP" };

	class CSVReader {
	private:
		ifstream fin;
		int lineno, colno;
		string header;
		string buf;

	public:
		CSVReader(const string &filename) {
			open(filename);
			if (fin.is_open()) {
				lineno = 0; colno = 0;
				header = nextLine();
			}
			else {
				lineno = -1; colno = -1;
				throw new exception("FileOpenFailedException");
			}
		}

		CSVReader() {
			lineno = -1; colno = -1;
		}

		void open(const string &filename) {
			if (fin.is_open())
				fin.close();
			fin.open(filename, ifstream::in);
			lineno = 0; colno = 0;
			if (!fin.is_open()) {
				lineno = -1; colno = -1;
				throw new exception("FileOpenFailedException");
			}
		}

		void close() {
			if (fin.is_open()) {
				lineno = -1; colno = -1;
				fin.close();
			}
		}

		bool isOpen() {
			return fin.is_open();
		}

		bool eof() {
			return fin.eof();
		}

		bool nextLine() {
			if (!fin.is_open())
				throw new exception("FileNotOpenedException");

			if (!fin.eof()) {
				getline(fin, buf);
				lineno++; colno = 0;
			} else
				return false;
		}
	};
}