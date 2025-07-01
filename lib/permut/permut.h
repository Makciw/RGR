#ifndef PERMUT_H
#define PERMUT_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <stdexcept>

using namespace std;

const int COLUMNS = 100;

#ifdef __cplusplus
extern "C" {
#endif

vector<vector<unsigned char>> inversePermuteColumns(const vector<vector<unsigned char>>& matrix, const vector<int>& colKey);
vector<vector<unsigned char>> inversePermuteRows(const vector<vector<unsigned char>>& matrix, const vector<int>& rowKey);
vector<vector<int>> readNumericMatrix(const string& filename);
void writeNumericMatrix(const string& filename, const vector<vector<int>>& matrix);
vector<int> generateKey(int size);
void saveKey(const string& filename, const vector<int>& key);
vector<int> readKey(const string& filename);
vector<vector<unsigned char>> createMatrix(const vector<unsigned char>& data, int& originalSize);
vector<vector<int>> matrixToNumeric(const vector<vector<unsigned char>>& matrix);
vector<unsigned char> matrixToBinary(const vector<vector<unsigned char>>& matrix, int originalSize);
vector<vector<unsigned char>> permuteColumns(const vector<vector<unsigned char>>& matrix, const vector<int>& colKey);
vector<vector<unsigned char>> permuteRows(const vector<vector<unsigned char>>& matrix, const vector<int>& rowKey);
vector<vector<int>> encryptData(const vector<unsigned char>& data, const vector<int>& colKey, const vector<int>& rowKey, int& originalSize);
vector<unsigned char> decryptData(const vector<vector<int>>& numericMatrix, const vector<int>& colKey, const vector<int>& rowKey, int originalSize);

#ifdef __cplusplus
}
#endif

#endif
