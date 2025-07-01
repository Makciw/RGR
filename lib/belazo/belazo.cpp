#include "belazo.h"

const int ALPHABET_SIZE = 256;
string belazoKey;
map<char, vector<char>> belazoTable;
bool tableGenerated = false;

void generateBelazoTable() {
    if (tableGenerated) return;

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        char c = static_cast<char>(i);
        vector<char> row;

        for (int j = 0; j < ALPHABET_SIZE; ++j) {
            char shifted = static_cast<char>((i + j) % ALPHABET_SIZE);
            row.push_back(shifted);
        }

        belazoTable[c] = row;
    }

    tableGenerated = true;
}

void setBelazoKey(const string& key) {
    if (key.empty()) throw invalid_argument("Ключ не может быть пустым");
    belazoKey = key;
    generateBelazoTable();
}

vector<int> belazoEncryptToNumbers(const vector<unsigned char>& data) {
    if (!tableGenerated) throw logic_error("Таблица не сгенерирована");
    if (belazoKey.empty()) throw logic_error("Ключ не установлен");

    vector<int> result;
    size_t keyLen = belazoKey.length();

    for (size_t i = 0; i < data.size(); ++i) {
        char currentKeyChar = belazoKey[i % keyLen];
        char plainChar = data[i];
        char encryptedChar = belazoTable.at(currentKeyChar)[static_cast<unsigned char>(plainChar)];
        result.push_back(static_cast<unsigned char>(encryptedChar));
    }

    return result;
}

vector<unsigned char> belazoDecryptFromNumbers(const vector<int>& encryptedNumbers) {
    if (!tableGenerated) throw logic_error("Таблица не сгенерирована");
    if (belazoKey.empty()) throw logic_error("Ключ не установлен");

    vector<unsigned char> result;
    size_t keyLen = belazoKey.length();

    for (size_t i = 0; i < encryptedNumbers.size(); ++i) {
        char currentKeyChar = belazoKey[i % keyLen];
        char encryptedChar = static_cast<char>(encryptedNumbers[i]);

        const auto& row = belazoTable.at(currentKeyChar);
        auto it = find(row.begin(), row.end(), encryptedChar);

        if (it == row.end()) throw runtime_error("Символ не найден в таблице");

        char decryptedChar = static_cast<char>(distance(row.begin(), it));
        result.push_back(decryptedChar);
    }

    return result;
}

string generateRandomKey(size_t length) {
    if (length == 0) throw invalid_argument("Длина ключа должна быть > 0");
	if (!tableGenerated) generateBelazoTable();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, ALPHABET_SIZE - 1);

    string key;
    for (size_t i = 0; i < length; ++i) {
        key += static_cast<unsigned char>(dis(gen));
    }
    return key;
}

void writeNumbersToFile(const string& filename, const vector<int>& numbers) {
    ofstream file(filename);
    if (!file) throw runtime_error("Не удалось открыть файл: " + filename);

    for (size_t i = 0; i < numbers.size(); ++i) {
        file << numbers[i];
        if (i != numbers.size() - 1) file << " ";
    }
}

vector<int> readNumbersFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) throw runtime_error("Не удалось открыть файл: " + filename);

    vector<int> numbers;
    int num;
    while (file >> num) {
        numbers.push_back(num);
    }
    return numbers;
}

void saveKey(const string filename, const string key) {
    ofstream file(filename);
    if (!file) throw runtime_error("Не удалось открыть файл: " + filename);
    file << key;
	file.close();
}

string readKeyBelozo(const string& filename) {
    ifstream file(filename);
    if (!file) throw runtime_error("Не удалось открыть файл: " + filename);

    string key;
    getline(file, key);
    return key;
}
