#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>

using namespace std;

namespace Cardiotocography {

	const string header_name[22] = { "LB", "AC", "FM", "UC",
		"DL", "DS", "DP", "ASTV", "MSTV", "ALTV", "MLTV", "Width",
		"Min", "Max", "Nmax", "Nzeros", "Mode", "Mean", "Median",
		"Variance", "Tendency", "NSP" };

	class CSVWriter {
	private:
		ofstream fout;
		string* buf;

	public:
		bool open(const string& filename) {
			fout.open(filename, ios::out | ios::trunc);

			if (!fout.is_open())
				return false;
			return true;
		}

		void write(const string& data, const int &size) {
			fout.write(data.data(), size);
			fout.flush();
		}

		void close() {
			fout.flush();
			fout.close();
		}
	};

	class CSVReader {
	private:
		ifstream fin;
		int lineno, colno;
		string header;
		string rawbuf; // Raw line buffer
		string* buf; // Dynamic splited line buffer

	public:
		CSVReader(const string& filename) {
			open(filename);
			buf = nullptr;
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
			buf = nullptr;
		}

		void open(const string& filename) {
			if (fin.is_open()) {
				delete[] buf;
				fin.close();
			}
			fin.open(filename, ifstream::in);
			lineno = 0; colno = 0;
			buf = nullptr;
			if (!fin.is_open()) {
				lineno = -1; colno = -1;
				throw new exception("FileOpenFailedException");
			}
		}

		void close() {
			if (fin.is_open()) {
				lineno = -1; colno = -1;
				delete[] buf;
				buf = nullptr;
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
				if (buf != nullptr)
					delete[] buf;
				buf = new string[22];

				string linebuf;
				getline(fin, linebuf);
				rawbuf = string(linebuf);

				int idx = 0, nextdlmidx = 0;
				while ((nextdlmidx = linebuf.find(",")) != -1) {
					buf[idx++] = linebuf.substr(0, nextdlmidx);
					linebuf.erase(0, nextdlmidx + 1);
				}

				// Last one is NSP; which is Label data
				// but because of delimeter, couldn't be processed
				buf[21] = linebuf;
				lineno++; colno = 0;

				return true;
			}
			else
				return false;
		}

		template<typename T = double>
		T next() {
			if (strcmp(typeid(T).name(), "double") == 0 ||
				strcmp(typeid(T).name(), "float") == 0)
				return atof(buf[colno++].data());
			else if (strcmp(typeid(T).name(), "int") == 0)
				return atoi(buf[colno++].data());
			else
				throw new exception("TemplateFormNotSupportedException");
		}

		string * currentLine() {
			string* line = new string[22]; // Copy array itself
			memcpy(line, buf, 22 * sizeof(string));
			return line;
		}

		string currentLineFull() {
			return rawbuf;
		}
	};
}