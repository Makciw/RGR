#include "permut.h"

vector<vector<unsigned char>> inversePermuteColumns(const vector<vector<unsigned char>>& matrix, const vector<int>& colKey) {
    if (colKey.size() != COLUMNS) {
        throw runtime_error("Неверный размер ключа столбцов");
    }

    vector<vector<unsigned char>> result(matrix.size(), vector<unsigned char>(COLUMNS));

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            result[i][colKey[j] - 1] = matrix[i][j];
        }
    }

    return result;
}

vector<vector<unsigned char>> inversePermuteRows(const vector<vector<unsigned char>>& matrix, const vector<int>& rowKey) {
    if (rowKey.size() != matrix.size()) {
        throw runtime_error("Неверный размер ключа строк");
    }

    vector<vector<unsigned char>> result(matrix.size(), vector<unsigned char>(COLUMNS));

    for (size_t i = 0; i < matrix.size(); ++i) {
        result[rowKey[i] - 1] = matrix[i];
    }

    return result;
}

vector<vector<int>> readNumericMatrix(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл: " + filename);
    }

    vector<vector<int>> matrix;
    string line;

    while (getline(file, line)) {
        vector<int> row;
        istringstream iss(line);
        int num;
        while (iss >> num) {
            row.push_back(num);
        }

        if (!row.empty()) {
            if (row.size() != COLUMNS) {
                throw runtime_error("Неверный формат матрицы в файле");
            }
            matrix.push_back(row);
        }
    }

    return matrix;
}

void writeNumericMatrix(const string& filename, const vector<vector<int>>& matrix) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл: " + filename);
    }

    for (const auto& row : matrix) {
        for (int num : row) {
            file << setw(4) << num << " ";
        }
        file << "\n";
    }
}

vector<int> generateKey(int size) {
    vector<int> key(size);
    for (int i = 0; i < size; ++i) {
        key[i] = i + 1;
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(key.begin(), key.end(), g);
    return key;
}

void saveKey(const string& filename, const vector<int>& key) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл: " + filename);
    }

    for (int num : key) {
        file << num << " ";
    }
}

vector<int> readKey(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл: " + filename);
    }

    vector<int> key;
    int num;
    while (file >> num) {
        key.push_back(num);
    }

    return key;
}

vector<vector<unsigned char>> createMatrix(const vector<unsigned char>& data, int& originalSize) {
    originalSize = static_cast<int>(data.size());
    int rows = (originalSize + COLUMNS - 1) / COLUMNS;
    vector<vector<unsigned char>> matrix(rows, vector<unsigned char>(COLUMNS, 0));

    for (int i = 0; i < originalSize; ++i) {
        matrix[i / COLUMNS][i % COLUMNS] = data[i];
    }

    return matrix;
}

vector<vector<int>> matrixToNumeric(const vector<vector<unsigned char>>& matrix) {
    vector<vector<int>> numericMatrix(matrix.size(), vector<int>(COLUMNS));
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            numericMatrix[i][j] = static_cast<int>(matrix[i][j]);
        }
    }
    return numericMatrix;
}

vector<unsigned char> matrixToBinary(const vector<vector<unsigned char>>& matrix, int originalSize) {
    vector<unsigned char> binaryData;
    binaryData.reserve(originalSize);

    for (size_t i = 0; i < matrix.size() && binaryData.size() < static_cast<size_t>(originalSize); ++i) {
        for (int j = 0; j < COLUMNS && binaryData.size() < static_cast<size_t>(originalSize); ++j) {
            binaryData.push_back(matrix[i][j]);
        }
    }

    return binaryData;
}

vector<vector<unsigned char>> permuteColumns(const vector<vector<unsigned char>>& matrix, const vector<int>& colKey) {
    if (colKey.size() != COLUMNS) {
        throw runtime_error("Неверный размер ключа столбцов");
    }

    vector<vector<unsigned char>> result(matrix.size(), vector<unsigned char>(COLUMNS));

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            result[i][j] = matrix[i][colKey[j] - 1];
        }
    }

    return result;
}

vector<vector<unsigned char>> permuteRows(const vector<vector<unsigned char>>& matrix, const vector<int>& rowKey) {
    if (rowKey.size() != matrix.size()) {
        throw runtime_error("Неверный размер ключа строк");
    }

    vector<vector<unsigned char>> result(matrix.size(), vector<unsigned char>(COLUMNS));

    for (size_t i = 0; i < matrix.size(); ++i) {
        result[i] = matrix[rowKey[i] - 1];
    }

    return result;
}

vector<vector<int>> encryptData(const vector<unsigned char>& data, const vector<int>& colKey, const vector<int>& rowKey, int& originalSize) {
    auto matrix = createMatrix(data, originalSize);
    matrix = permuteColumns(matrix, colKey);
    matrix = permuteRows(matrix, rowKey);
    return matrixToNumeric(matrix);
}

vector<unsigned char> decryptData(const vector<vector<int>>& numericMatrix, const vector<int>& colKey, const vector<int>& rowKey, int originalSize) {
    vector<vector<unsigned char>> matrix(numericMatrix.size(), vector<unsigned char>(COLUMNS));

    for (size_t i = 0; i < numericMatrix.size(); ++i) {
        for (int j = 0; j < COLUMNS; ++j) {
            matrix[i][j] = static_cast<unsigned char>(numericMatrix[i][j]);
        }
    }

    matrix = inversePermuteRows(matrix, rowKey);
    matrix = inversePermuteColumns(matrix, colKey);

    return matrixToBinary(matrix, originalSize);
}
