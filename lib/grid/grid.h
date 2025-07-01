#ifndef GRID_H
#define GRID_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <stdexcept>

using namespace std;

const int GRID_SIZE = 10;

#ifdef __cplusplus
extern "C" {
#endif

vector<vector<bool>> makeOriginalMatrix();
bool isValidKey(const vector<vector<bool>>& key);
vector<vector<bool>> rotateKey(const vector<vector<bool>>& key);
vector<vector<int>> encryptBlock(const vector<unsigned char>& block, const vector<vector<bool>>& key);
vector<unsigned char> decryptBlock(const vector<vector<int>>& encryptedMatrix, const vector<vector<bool>>& key);
void writeKeyToFile(const string& filename, const vector<vector<bool>>& key);
vector<vector<bool>> readKeyFromFile(const string& filename);
void writeEncryptedMatrices(const string& filename, const vector<vector<vector<int>>>& matrices);
vector<vector<vector<int>>> readEncryptedMatrices(const string& filename);

#ifdef __cplusplus
}
#endif

#endif
