#pragma once
#define HEADER
#ifdef HEADER

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <dlfcn.h>
#include <memory>
#include <clocale>
#include <map>
#include <stdexcept>
#include <cstring>
#include <exception>
#include <codecvt>
#if __cplusplus >= 201703L
#include <locale>
#include <codecvt>
#endif

using namespace std;

extern random_device rd;
extern mt19937 gen;

const int BLOCK_SIZE = 100;
const int COLUMNS = 100;

enum class OperationType {
    ENCRYPT = 1,
    DECRYPT = 2,
	EXITE = 3
};

enum class AlgorithmType {
    GRID = 1,
    PERMUTATION = 2,
    BELAZO = 3
};

enum class DataSource {
    CONSOLE = 0,
    FILE = 1
};

enum class KeySource {
    FILE = 0,
    GENERATE = 1
};

//help
void chekInput(int& input, int step);
vector<unsigned char> readBinaryFile(const string& filename);
void writeBinaryFile(const string& filename, const vector<unsigned char>& data);
string wstringToUtf8(const wstring& str);
wstring utf8ToWstring(const string& str);
OperationType getOperationType();
DataSource getInputSource();
AlgorithmType getAlgorithmType();
KeySource getKeySource();
#endif
