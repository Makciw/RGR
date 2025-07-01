#include "Header.h"

string wstringToUtf8(const wstring& str) {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(str);
}

void chekInput(int& input, int step) {
    if (wcin.fail()) {
        wcin.clear();
        wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
        throw invalid_argument("Ошибка ввода. Введите число.");
    }
    if (input != 0 && input != 1 && step == 2) {
        throw invalid_argument("Неверный выбор. Допустимые значения: 0, 1");
    }
    if (input != 1 && input != 2 && input != 3 && step == 3) {
        throw invalid_argument("Неверный выбор. Допустимые значения: 0, 1");
    }
}

OperationType  getOperationType() {
    int operation;
    bool valid = false;
    
    while (!valid) {
        try {
            wcout << L"Выберите действия:" << endl;
            wcout << L"Операция:" << endl;
            wcout << L"\t1 - шифрование, 2 - дешифрование, 3 - выход: ";
            wcin >> operation;
            wcin.ignore();
            chekInput(operation, 3);
            valid = true;
        } catch (const invalid_argument& e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }
    return static_cast<OperationType>(operation);
}

DataSource  getInputSource() {
    int source;
    bool valid = false;
    
    while (!valid) {
        try {
            wcout << L"\nВыберите как будут введены данные: " << endl;
            wcout << L"\t0 - текст, 1 - чтение из файла: ";
            wcin >> source;
            wcin.ignore();
            chekInput(source, 2);
            valid = true;
        } catch (const invalid_argument& e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }
    return static_cast<DataSource>(source);
}

AlgorithmType  getAlgorithmType() {
    int alg;
    bool valid = false;
    
    while (!valid) {
        try {
            wcout << L"\nВыберите шифр: " << endl;
            wcout << L"\t1 - шифр решеткой, 2 - двойная табличная перестановка, 3 - шифр Белазо: ";
            wcin >> alg;
            wcin.ignore();
            chekInput(alg, 3);
            valid = true;
        } catch (const invalid_argument& e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }
    return static_cast<AlgorithmType>(alg);
}

KeySource  getKeySource() {
    int key;
    bool valid = false;
    
    while (!valid) {
        try {
            wcout << L"\nКлюч: " << endl;
            wcout << L"\t0 - предоставить свой, 1 - сгенерировать автоматически: ";
            wcin >> key;
            wcin.ignore();
            chekInput(key, 2);
            valid = true;
        } catch (const invalid_argument& e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }
    return static_cast<KeySource>(key);
}

void writeBinaryFile(const string& filename, const vector<unsigned char>& data) {
    ofstream file(filename, ios::binary);
    if (!file) throw runtime_error("Не удалось открыть файл: " + filename);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    file.close();
}

vector<unsigned char> readBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    if (!file) throw runtime_error("Не удалось открыть файл: " + filename);

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw runtime_error("Ошибка чтения файла: " + filename);
    }
    file.close();
    return buffer;
}

wstring utf8ToWstring(const string& str) {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
}
