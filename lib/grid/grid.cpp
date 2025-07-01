#include "grid.h"

mt19937 gen;

vector<vector<bool>> makeOriginalMatrix() {
    uniform_int_distribution<> index(0, GRID_SIZE - 1);
    vector<vector<bool>> originalMatrix(GRID_SIZE, vector<bool>(GRID_SIZE, true));

    int sumZero = 0;

    while (sumZero != 25) {
        int i = index(gen);
        int j = index(gen);
        if (originalMatrix[i][j] == true &&
            originalMatrix[j][GRID_SIZE - i - 1] == true &&
            originalMatrix[GRID_SIZE - i - 1][GRID_SIZE - j - 1] == true &&
            originalMatrix[GRID_SIZE - j - 1][i] == true) {
            originalMatrix[i][j] = false;
        }

        sumZero = 0;
        for_each(originalMatrix.begin(), originalMatrix.end(), [&sumZero](vector<bool> colon) {
            sumZero += count_if(colon.begin(), colon.end(), [](bool part) {
                return !part;
                });
            });
    }
    return originalMatrix;
}

bool isValidKey(const vector<vector<bool>>& key) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (key[i][j] == false && key[j][GRID_SIZE - i - 1] == true && key[GRID_SIZE - i - 1][GRID_SIZE - j - 1] == true && key[GRID_SIZE - j - 1][i] == true) {
                continue;
            }
            else return false;
        }
    }
    return true;
}

vector<vector<bool>> rotateKey(const vector<vector<bool>>& key) {
    vector<vector<bool>> rotated(GRID_SIZE, vector<bool>(GRID_SIZE));
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            rotated[j][GRID_SIZE - 1 - i] = key[i][j];
        }
    }
    return rotated;
}

vector<vector<int>> encryptBlock(const vector<unsigned char>& block, const vector<vector<bool>>& key) {
    vector<vector<unsigned char>> grid(GRID_SIZE, vector<unsigned char>(GRID_SIZE, 0));
    vector<vector<bool>> currentKey = key;
    size_t dataIndex = 0;

    for (int rotation = 0; rotation < 4; ++rotation) {
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (!currentKey[i][j] && dataIndex < block.size()) {
                    grid[i][j] = block[dataIndex++];
                }
            }
        }
        currentKey = rotateKey(currentKey);
    }

    vector<vector<int>> result(GRID_SIZE, vector<int>(GRID_SIZE));
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            result[i][j] = static_cast<int>(grid[i][j]);
        }
    }

    return result;
}

vector<unsigned char> decryptBlock(const vector<vector<int>>& encryptedMatrix, const vector<vector<bool>>& key) {
    vector<unsigned char> result;
    vector<vector<bool>> currentKey = key;

    for (int rotation = 0; rotation < 4; ++rotation) {
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (!currentKey[i][j]) {
                    result.push_back(static_cast<unsigned char>(encryptedMatrix[i][j]));
                }
            }
        }
        currentKey = rotateKey(currentKey);
    }

    return result;
}

void writeKeyToFile(const string& filename, const vector<vector<bool>>& key) {
    ofstream file(filename);
    if (!file) {
        throw runtime_error("Cannot open key file: " + filename);
    }

    for (const auto& row : key) {
        for (bool val : row) {
            file << (val ? '1' : '0');
        }
        file << '\n';
    }
}

vector<vector<bool>> readKeyFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Cannot open key file: " + filename);
    }

    vector<vector<bool>> key(GRID_SIZE, vector<bool>(GRID_SIZE));
    string line;
    int row = 0;

    while (getline(file, line) && row < GRID_SIZE) {
        if (line.size() != GRID_SIZE) {
            throw runtime_error("Invalid key format in file");
        }

        for (int col = 0; col < GRID_SIZE; ++col) {
            if (line[col] == '0') key[row][col] = false;
            else if (line[col] == '1') key[row][col] = true;
            else throw runtime_error("Invalid character in key file");
        }
        row++;
    }

    if (row != GRID_SIZE) {
        throw runtime_error("Incomplete key in file");
    }
    if (isValidKey(key)) {
        throw runtime_error("некорректный ключ");
    }
    return key;
}

void writeEncryptedMatrices(const string& filename, const vector<vector<vector<int>>>& matrices) {
    ofstream file(filename);
    if (!file) {
        throw runtime_error("Cannot open output file: " + filename);
    }

    for (const auto& matrix : matrices) {
        for (const auto& row : matrix) {
            for (int val : row) {
                file << setw(3) << val << " ";
            }
            file << '\n';
        }
        file << '\n';
    }
}

vector<vector<vector<int>>> readEncryptedMatrices(const string& filename) {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Cannot open input file: " + filename);
    }

    vector<vector<vector<int>>> matrices;
    vector<vector<int>> currentMatrix(GRID_SIZE, vector<int>(GRID_SIZE));
    int row = 0;
    string line;

    while (getline(file, line)) {
        if (line.empty()) {
            if (row == GRID_SIZE) {
                matrices.push_back(currentMatrix);
                currentMatrix = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE));
                row = 0;
            }
            continue;
        }

        if (row >= GRID_SIZE) {
            throw runtime_error("Invalid matrix format in file");
        }

        istringstream iss(line);
        for (int col = 0; col < GRID_SIZE; ++col) {
            if (!(iss >> currentMatrix[row][col])) {
                throw runtime_error("Invalid matrix data in file");
            }
        }
        row++;
    }

    if (row == GRID_SIZE) {
        matrices.push_back(currentMatrix);
    }

    return matrices;
}
