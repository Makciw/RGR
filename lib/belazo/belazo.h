#ifndef BELAZO_H
#define BELAZO_H

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <stdexcept>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

void generateBelazoTable();
void setBelazoKey(const string& key);
vector<int> belazoEncryptToNumbers(const vector<unsigned char>& data);
vector<unsigned char> belazoDecryptFromNumbers(const vector<int>& encryptedNumbers);
string generateRandomKey(size_t length);
void writeNumbersToFile(const string& filename, const vector<int>& numbers);
vector<int> readNumbersFromFile(const string& filename);
void saveKey(const string filename, const string key);
string readKeyBelozo(const string& filename);

#ifdef __cplusplus
}
#endif

#endif
