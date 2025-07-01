#include "Header.h"

random_device rd;
mt19937 gen(rd());

int main() {
	locale::global(locale(""));
        wcout.imbue(locale());
        wcin.imbue(locale());

        setlocale(LC_ALL, "ru_RU.UTF-8");

	typedef vector<vector<bool>> (*MakeOriginalMatrixFunc)();
	typedef vector<vector<bool>> (*ReadKeyFromFileFunc)(const string);
	typedef vector<vector<int>>(*EncryptBlockFunc)(const vector<unsigned char>&, const vector<vector<bool>>&);
	typedef vector<unsigned char>(*DecryptBlockFunc)(const vector<vector<int>>&, const vector<vector<bool>>&);
	typedef void (*WriteKeyToFileFunc)(const string&, const vector<vector<bool>>&);
	typedef vector<vector<vector<int>>>(*ReadEncryptedMatricesFunc)(const string&);
	typedef void (*WriteEncryptedMatricesFunc)(const string&, const vector<vector<vector<int>>>&);
	
	void* handleGrid = dlopen("./libgrid.so", RTLD_LAZY);
	if (!handleGrid){
		wcerr << L"grid librory error" << endl;
	return 1;
	}
	dlerror();
	MakeOriginalMatrixFunc makeOriginalMatrix = (MakeOriginalMatrixFunc)dlsym(handleGrid, "makeOriginalMatrix");
	ReadKeyFromFileFunc readKeyFromFile = (ReadKeyFromFileFunc)dlsym(handleGrid, "readKeyFromFile");
	EncryptBlockFunc encryptBlock = (EncryptBlockFunc)dlsym(handleGrid, "encryptBlock");
	DecryptBlockFunc decryptBlock = (DecryptBlockFunc)dlsym(handleGrid, "decryptBlock");
	WriteKeyToFileFunc writeKeyToFile = (WriteKeyToFileFunc)dlsym(handleGrid, "writeKeyToFile");
	ReadEncryptedMatricesFunc readEncryptedMatrices = (ReadEncryptedMatricesFunc)dlsym(handleGrid, "readEncryptedMatrices");
	WriteEncryptedMatricesFunc writeEncryptedMatrices = (WriteEncryptedMatricesFunc)dlsym(handleGrid, "writeEncryptedMatrices");

	typedef vector<int>(*GenerateKeyFunc)(int);
	typedef vector<int>(*ReadKeyFunc)(const string&);
	typedef void (*SaveKeyFunc)(const string&, const vector<int>&);
	typedef vector<vector<int>>(*EncryptDataFunc)(const vector<unsigned char>&, const vector<int>&, const vector<int>&, int&);
	typedef vector<unsigned char>(*DecryptDataFunc)(const vector<vector<int>>&, const vector<int>&, const vector<int>&, int);
	typedef vector<vector<int>>(*ReadNumericMatrixFunc)(const string&);
	typedef void (*WriteNumericMatrixFunc)(const string&, const vector<vector<int>>&);

	void* handlePermut = dlopen("./libpermut.so", RTLD_LAZY);
	if (!handlePermut) {
    		wcerr << L"permut librory error" << endl;
    		return 1;
	}
	dlerror();
	GenerateKeyFunc generateKey = (GenerateKeyFunc)dlsym(handlePermut, "generateKey");
	ReadKeyFunc readKey = (ReadKeyFunc)dlsym(handlePermut, "readKey");
	SaveKeyFunc saveKey = (SaveKeyFunc)dlsym(handlePermut, "saveKey");
	EncryptDataFunc encryptData = (EncryptDataFunc)dlsym(handlePermut, "encryptData");
	DecryptDataFunc decryptData = (DecryptDataFunc)dlsym(handlePermut, "decryptData");
	ReadNumericMatrixFunc readNumericMatrix = (ReadNumericMatrixFunc)dlsym(handlePermut, "readNumericMatrix");
	WriteNumericMatrixFunc writeNumericMatrix = (WriteNumericMatrixFunc)dlsym(handlePermut, "writeNumericMatrix");

	const char* DlsymError = dlerror();
	if (DlsymError){
		wcerr << DlsymError << endl;
		return 1;
	}

	typedef void (*SetBelazoKeyFunc)(const string&);
	typedef vector<int>(*BelazoEncryptToNumbersFunc)(const vector<unsigned char>&);
	typedef vector<unsigned char>(*BelazoDecryptFromNumbersFunc)(const vector<int>&);
	typedef string(*GenerateRandomKeyFunc)(size_t);
	typedef void (*WriteNumbersToFileFunc)(const string&, const vector<int>&);
	typedef vector<int>(*ReadNumbersFromFileFunc)(const string&);
	typedef string(*ReadKeyBelozoFunc)(const string&);

	void* handleBelazo = dlopen("./libbelazo.so", RTLD_LAZY);
	if (!handleBelazo) {
	    wcerr << L"belazo library erorr" << endl;
	    return 1;
	}
	dlerror();
	SetBelazoKeyFunc setBelazoKey = (SetBelazoKeyFunc)dlsym(handleBelazo, "setBelazoKey");
	BelazoEncryptToNumbersFunc belazoEncryptToNumbers = (BelazoEncryptToNumbersFunc)dlsym(handleBelazo, "belazoEncryptToNumbers");
	BelazoDecryptFromNumbersFunc belazoDecryptFromNumbers = (BelazoDecryptFromNumbersFunc)dlsym(handleBelazo, "belazoDecryptFromNumbers");
	GenerateRandomKeyFunc generateRandomKey = (GenerateRandomKeyFunc)dlsym(handleBelazo, "generateRandomKey");
	WriteNumbersToFileFunc writeNumbersToFile = (WriteNumbersToFileFunc)dlsym(handleBelazo, "writeNumbersToFile");
	ReadNumbersFromFileFunc readNumbersFromFile = (ReadNumbersFromFileFunc)dlsym(handleBelazo, "readNumbersFromFile");
	ReadKeyBelozoFunc readKeyBelozo = (ReadKeyBelozoFunc)dlsym(handleBelazo, "readKeyBelozo");

	while (true){
	OperationType operation = getOperationType();
	if (operation == OperationType::EXITE){
                break;
        }

	DataSource textSource = (operation == OperationType::ENCRYPT) ? getInputSource() : DataSource::FILE;
    	AlgorithmType alg = getAlgorithmType();
    	KeySource keySource = (operation == OperationType::ENCRYPT) ? getKeySource() : KeySource::FILE;

	vector<int> colKey, rowKey;
	vector<vector<bool>> matrixKey(10, vector<bool>(10));
	string belazoKey = "hello";

	if (keySource == KeySource::FILE && operation == OperationType::ENCRYPT) {
		wstring wfileKeyGrid, wfileKey1Permut, wfileKey2Permut, wfileKeyBelazo;
		switch (alg){
		case AlgorithmType::GRID: {
			wcout << L"\nВведите путь до ключа: ";
			getline(wcin, wfileKeyGrid);

			string fileKeyGrid = wstringToUtf8(wfileKeyGrid);

			matrixKey = readKeyFromFile(fileKeyGrid);
			break;
		}
		case AlgorithmType::PERMUTATION: {
			wcout << L"\nВведите имя файла где хранится ключ столбцов: ";
			getline(wcin, wfileKey1Permut);

			wcout << L"\nВведите имя файла где хранится ключ строк: ";
			getline(wcin, wfileKey2Permut);

			string fileKey1Permut = wstringToUtf8(wfileKey1Permut);
			string fileKey2Permut = wstringToUtf8(wfileKey2Permut);

			colKey = readKey(fileKey1Permut);
			rowKey = readKey(fileKey2Permut);
			break;
		}
		case AlgorithmType::BELAZO: {
			wcout << L"\nФайл с ключом: ";
			getline(wcin, wfileKeyBelazo);

			string fileKeyBelazo = wstringToUtf8(wfileKeyBelazo);

			belazoKey = readKeyBelozo(fileKeyBelazo);
			break;
		}
		}
	}

	if (keySource == KeySource::GENERATE && operation != OperationType::DECRYPT) {
		if (alg == AlgorithmType::GRID) {
			matrixKey = makeOriginalMatrix();
		}
		else if (alg == AlgorithmType::BELAZO) {
			int length;
			wcout << L"\nДлина ключа: ";
			wcin >> length;
			wcin.ignore();

			belazoKey = generateRandomKey(length);
		}
	}

	if (operation == OperationType::ENCRYPT) {
		if (textSource == DataSource::CONSOLE) {
			wstring textWstr, fileWname;
			wcout << L"\nВведите файл для выходных данных: ";
			getline(wcin, fileWname);
			wcout << L"\nВведите текст: ";
			getline(wcin, textWstr);
			
			string fileName = wstringToUtf8(fileWname);
			string textStr = wstringToUtf8(textWstr);
			switch (alg){
			case AlgorithmType::GRID: {
				string keyFilename = "key.txt";
				writeKeyToFile(keyFilename, matrixKey);
				vector<vector<unsigned char>> blocks;
				for (size_t i = 0; i < textStr.size(); i += BLOCK_SIZE) {
					auto last = min(textStr.size(), i + BLOCK_SIZE);
					blocks.emplace_back(textStr.begin() + i, textStr.begin() + last);
				}
				vector<vector<vector<int>>> encryptedMatrices;
				for (const auto& block : blocks) {
					encryptedMatrices.push_back(encryptBlock(block, matrixKey));
				}
				writeEncryptedMatrices(fileName, encryptedMatrices);
				wcout << L"\nФайл успешно зашифрован " << fileWname <<  L" Ключи сохранены " << utf8ToWstring(keyFilename) << endl;
				break;
			}
			case AlgorithmType::PERMUTATION: {
				wstring wcolKeyFile, wrowKeyFile;
				int originalSize;
				wcout << L"Введите имя файла для ключа столбцов: ";
				getline(wcin, wcolKeyFile);

				wcout << L"Введите имя файла для ключа строк: ";
				getline(wcin, wrowKeyFile);

				string colKeyFile = wstringToUtf8(wcolKeyFile);
				string rowKeyFile = wstringToUtf8(wrowKeyFile);

				vector<unsigned char> data;
				for (auto i : textStr) {
					data.push_back(static_cast<unsigned char>(i));
				}
				if (colKey.empty()) {
					colKey = generateKey(COLUMNS);
					rowKey = generateKey((data.size() + COLUMNS - 1) / COLUMNS);
				}
				saveKey(colKeyFile, colKey);
				saveKey(rowKeyFile, rowKey);

				auto encryptedMatrix = encryptData(data, colKey, rowKey, originalSize);
				writeNumericMatrix(fileName, encryptedMatrix);

				ofstream sizeFile("text.size");
				sizeFile << originalSize;

				wcout << L"\nФайл успешно зашифрован " << fileWname << L" Ключи сохранены " << wcolKeyFile << " " << wrowKeyFile << endl;
				break;
			}
			case AlgorithmType::BELAZO: {
				wstring wkeyFile;
				wcout << L"\nФайл для сохранения ключа: ";
	                        getline(wcin, wkeyFile);
				
				string keyFile = wstringToUtf8(wkeyFile);
				ofstream file(keyFile);
                	        file << belazoKey;
        	                file.close();

				setBelazoKey(belazoKey);
				vector<unsigned char> data;
				for (unsigned char i : textStr) {
					data.push_back(i);
				}
				auto encryptedNumbers = belazoEncryptToNumbers(data);
				writeNumbersToFile(fileName, encryptedNumbers);
				wcout << L"\nФайл успешно зашифрован " << fileWname << L" Ключи сохранены " << wkeyFile << endl;
				break;
			}
			}
		}
		else {
			wstring winputFile, woutputFile;
			wcout << L"\nВведите путь до файла: ";
			getline(wcin, winputFile);

			wcout << L"\nВведите файл для выходных данных: ";
			getline(wcin, woutputFile);
			
			string inputFile = wstringToUtf8(winputFile);
			string outputFile = wstringToUtf8(woutputFile);

			vector<unsigned char> data = readBinaryFile(inputFile);
			switch (alg){
			case AlgorithmType::GRID: {
				string keyFilename = "key.txt";
				writeKeyToFile(keyFilename, matrixKey);
				vector<vector<unsigned char>> blocks;
				for (size_t i = 0; i < data.size(); i += BLOCK_SIZE) {
					auto last = min(data.size(), i + BLOCK_SIZE);
					blocks.emplace_back(data.begin() + i, data.begin() + last);
				}
				vector<vector<vector<int>>> encryptedMatrices;
				for (const auto& block : blocks) {
					encryptedMatrices.push_back(encryptBlock(block, matrixKey));
				}
				writeEncryptedMatrices(outputFile, encryptedMatrices);
				wcout << L"\nФайл успешно зашифрован " << woutputFile <<  L"Ключи сохранены " << utf8ToWstring(keyFilename) << endl;
				break;
			}
			case AlgorithmType::PERMUTATION: {
				wstring wcolKeyFile, wrowKeyFile;
				int originalSize;
				wcout << L"\nВведите имя файла для ключа столбцов: ";
				getline(wcin, wcolKeyFile);

				wcout << L"\nВведите имя файла для ключа строк: ";
				getline(wcin, wrowKeyFile);

				string colKeyFile = wstringToUtf8(wcolKeyFile);
				string rowKeyFile = wstringToUtf8(wrowKeyFile);

				auto colKey = generateKey(COLUMNS);
				auto rowKey = generateKey((data.size() + COLUMNS - 1) / COLUMNS);
				saveKey(colKeyFile, colKey);
				saveKey(rowKeyFile, rowKey);

				auto encryptedMatrix = encryptData(data, colKey, rowKey, originalSize);
				writeNumericMatrix(outputFile, encryptedMatrix);

				ofstream sizeFile(outputFile + ".size");
				sizeFile << originalSize;

				wcout << L"\nФайл успешно зашифрован " << woutputFile << L" Ключи сохранены " << wcolKeyFile << " " << wrowKeyFile << endl;
				break;
			}
			case AlgorithmType::BELAZO: {
				wstring wkeyFile;
                                wcout << L"\nФайл для сохранения ключа: ";
                                getline(wcin, wkeyFile);

                                string keyFile = wstringToUtf8(wkeyFile);
				ofstream file(keyFile);
                                file << belazoKey;
                                file.close();

				setBelazoKey(belazoKey);
				auto encryptedNumbers = belazoEncryptToNumbers(data);
				writeNumbersToFile(outputFile, encryptedNumbers);

				wcout << L"\nФайл успешно зашифрован " << woutputFile << L" Ключи сохранены " << wkeyFile << endl;

				break;
			}
			}
		}
	}
	else {
		wstring wkeyFile, winputFile, woutputFile;
		wcout << L"\nВведите путь до зашифрованного файла: ";
		getline(wcin, winputFile);
		wcout << L"\nВведите путь для выходных данных: ";
		getline(wcin, woutputFile);

		string inputFile = wstringToUtf8(winputFile);
		string outputFile = wstringToUtf8(woutputFile);
		switch (alg) {
		case AlgorithmType::GRID: {
			wcout << L"\nВведите путь до ключа (по умолчанию: key.txt): ";
			getline(wcin, wkeyFile);

			string keyFile = wstringToUtf8(wkeyFile);

			if (keyFile.empty()) keyFile = "key.txt";
			matrixKey = readKeyFromFile(keyFile);
			auto encryptedMatrices = readEncryptedMatrices(inputFile);
			vector<unsigned char> decryptedData;
			for (const auto& matrix : encryptedMatrices) {
				auto block = decryptBlock(matrix, matrixKey);
				decryptedData.insert(decryptedData.end(), block.begin(), block.end());
			}
			writeBinaryFile(outputFile, decryptedData);
			wcout << L"\nФайл расшифрован и записан: " << woutputFile << endl;
			break;
		}
		case AlgorithmType::PERMUTATION: {
			wstring wcolKeyFile, wrowKeyFile;
			int originalSize;
			wcout << L"\nВведите имя файла с ключом столбцов: ";
			getline(wcin, wcolKeyFile);

			wcout << L"\nВведите имя файла с ключом строк: ";
			getline(wcin, wrowKeyFile);

			string colKeyFile = wstringToUtf8(wcolKeyFile);
			string rowKeyFile = wstringToUtf8(wrowKeyFile);

			ifstream sizeFile(inputFile + ".size");
			if (!(sizeFile >> originalSize)) {
				throw runtime_error("Не удалось прочитать оригинальный размер файла");
			}

			auto encryptedMatrix = readNumericMatrix(inputFile);
			auto colKey = readKey(colKeyFile);
			auto rowKey = readKey(rowKeyFile);

			auto decryptedData = decryptData(encryptedMatrix, colKey, rowKey, originalSize);
			writeBinaryFile(outputFile, decryptedData);
			wcout << L"\nФайл расшифрован и записан: " << woutputFile << endl;

			break;
		}
		case AlgorithmType::BELAZO: {
			wcout << L"Файл с ключом: ";
			getline(wcin, wkeyFile);

			string keyFile = wstringToUtf8(wkeyFile);

			belazoKey = readKeyBelozo(keyFile);
			setBelazoKey(belazoKey);

			auto numbers = readNumbersFromFile(inputFile);
			auto decryptedData = belazoDecryptFromNumbers(numbers);
			writeBinaryFile(outputFile, decryptedData);

			wcout << L"\nФайл расшифрован и записан: " << woutputFile << endl;
			break;
		}
		}
	}
	}
	dlclose(handleGrid);
        dlclose(handlePermut);
        dlclose(handleBelazo);

	return 0;
}
