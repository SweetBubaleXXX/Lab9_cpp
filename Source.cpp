// cp1251
#include <iostream>
#include <iomanip>
#include <vector>
#include <Windows.h>

#define a_po_russki_mozhno setlocale(LC_ALL, "Russian"); system("chcp 1251")

using namespace std;

const int NAME_LENGTH = 20;

enum Menu { EXIT, WRITE, READ, PRINT, WRITE_TO_FILE };
enum Months { JAN = 1, FEB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AUG = 8, SEP = 9, OCT = 10, NOV = 11, DEC = 12 };

struct Semen {
    char name[NAME_LENGTH];
    int monthSowing;
    int monthSeating;
    int monthHarvesting;
};

int getNumberOfSiemens();

void getSiemens(Semen*, int);

void filterHarvesting(vector<Semen>&, Semen*, int, int);

int findGreaterNameIndex(vector<Semen>&, char*);

int writeNamesToBuffer(char*, int, vector<Semen>&);

void getMenuFunctionNumber(int*);

void showErrorAndExit(const char*);

void printMessage(const char*, const char*);

void writeMessageToFile(const char*, const char*);

template <typename T>
void writeFile(const char*, T*, int);

template <typename T>
T* readFile(const char*, int);

int main(int argc, char** argv) {
    a_po_russki_mozhno;

    if (argc == 1) showErrorAndExit("You should specify file path!");
    char* path = argv[1];

    int numberOfSiemens;
    Semen* siemens;
    vector<Semen> outputSiemens;

    int bufferLength;
    char* outputBuffer;

    int functionNumber = 1;
    while (functionNumber && !cin.fail()) {
        getMenuFunctionNumber(&functionNumber);
        switch (functionNumber) {
        case EXIT:
            functionNumber = 0;
            break;
        case WRITE:
            numberOfSiemens = getNumberOfSiemens();
            siemens = new Semen[numberOfSiemens];
            getSiemens(siemens, numberOfSiemens);
            writeFile<Semen>(path, &siemens[0], numberOfSiemens);
            delete[] siemens;
            break;
        case READ:
        case PRINT:
        case WRITE_TO_FILE:
            numberOfSiemens = getNumberOfSiemens();
            siemens = readFile<Semen>(path, numberOfSiemens);
            cout << "Культуры из файла:" << endl;
            for (int i = 0; i < numberOfSiemens; i++) cout << "\"" << siemens[i].name << "\"" << endl;
            if (functionNumber == READ) break;
            filterHarvesting(outputSiemens, siemens, numberOfSiemens, AUG);
            if (!outputSiemens.size()) { cout << "\nКультуры не найдены(" << endl; break; }
            bufferLength = outputSiemens.size() * (NAME_LENGTH + 2);
            outputBuffer = new char[bufferLength];
            writeNamesToBuffer(outputBuffer, bufferLength, outputSiemens);
            if (functionNumber == PRINT) { printMessage(outputBuffer, "Negative numbers"); }
            else if (functionNumber == WRITE_TO_FILE) writeMessageToFile(outputBuffer, path);
            outputSiemens.clear();
            delete[] siemens;
            delete[] outputBuffer;
            break;
        default:
            cout << "Invalid function number!" << endl;
            break;
        }
    }

    return 0;
}

int getNumberOfSiemens() {
    int numberOfSiemens;
    cout << "Введите количество культур: ";
    cin >> numberOfSiemens;
    if (cin.fail() || numberOfSiemens <= 0) { puts("Неверное число!"); return 1; }
    return numberOfSiemens;
}

void getSiemens(Semen* siemens, int numberOfSiemens) {
    for (int n = 0; n < numberOfSiemens; n++) {
        Semen* siemen = new Semen;

        cout << "{\n\tНазвание культуры: ";
        scanf_s("%s", siemen->name, NAME_LENGTH);
        cout << "\tМесяц посева: ";
        scanf_s("%d", &(siemen->monthSowing));
        cout << "\tМесяц выссадки рассады: ";
        scanf_s("%d", &(siemen->monthSeating));
        cout << "\tМесяц уборки: ";
        scanf_s("%d", &(siemen->monthHarvesting));
        cout << "}" << endl;

        siemens[n] = *siemen;
        delete siemen;
    }
}

void filterHarvesting(vector<Semen>& outputVector, Semen* inputArr, int arrayLength, int monthNumber) {
    for (int i = 0; i < arrayLength; i++) {
        if (inputArr[i].monthHarvesting != monthNumber) continue;
        int insertIndex = findGreaterNameIndex(outputVector, inputArr[i].name);
        outputVector.insert(outputVector.begin() + insertIndex, inputArr[i]);
    }
}

int findGreaterNameIndex(vector<Semen>& siemens, char* name) {
    if (!siemens.size()) return 0;
    if (siemens.size() == 1) return strcmp(name, siemens[0].name) > 0;
    int start = 0;
    int end = siemens.size();
    int curIndex = abs((start + end) / 2);
    while (end > start) {
        curIndex = abs((start + end) / 2);
        if (strcmp(name, siemens[curIndex].name) > 0) {
            start = curIndex + 1;
        }
        else {
            end = curIndex;
        }
    }
    return end;
}

int writeNamesToBuffer(char* buffer, int bufferLength, vector<Semen>& siemens) {
    int bufferPos = 0;
    for (int i = 0; i < siemens.size(); i++) {
        if (bufferPos >= bufferLength - sizeof(char[NAME_LENGTH])) {
            printMessage("Some values could be missing!!!", "Buffer overflow");
            return 1;
        }
        bufferPos += sprintf(buffer + bufferPos, "%s, ", siemens[i].name);
    }
    return 0;
}

void getMenuFunctionNumber(int* functionNumber) {
    cout << "Menu {" << endl;
    cout << setw(15) << right << "Exit: " << EXIT << endl;
    cout << setw(15) << right << "Write: " << WRITE << endl;
    cout << setw(15) << right << "Read: " << READ << endl;
    cout << setw(15) << right << "Print: " << PRINT << endl;
    cout << setw(15) << right << "Write to file: " << WRITE_TO_FILE << endl;
    cout << "}\nEnter function number >> ";
    cin >> *functionNumber;
    cin.clear();
    cin.ignore(INT16_MAX, '\n');
}

void showErrorAndExit(const char* errorMessage) {
    _fcloseall();
    cout << errorMessage << endl;
    exit(1);
}

void printMessage(const char* message, const char* windowTitle) {
    MessageBoxA(NULL, message, windowTitle, MB_OK);
}

void writeMessageToFile(const char* message, const char* path) {
    FILE* filePtr = fopen(path, "w");
    if (filePtr == NULL) return showErrorAndExit("Can't create file ='(");

    fputs(message, filePtr);
    if (ferror(filePtr)) return showErrorAndExit("Error while writing file ='(");

    fclose(filePtr);
}

template <typename T>
void writeFile(const char* path, T* dataForWriting, int amount = 1) {
    FILE* filePtr = fopen(path, "wb");
    if (filePtr == NULL) return showErrorAndExit("Can't create file ='(");

    fwrite(dataForWriting, sizeof(T), amount, filePtr);
    if (ferror(filePtr)) return showErrorAndExit("Error while writing file ='(");

    fclose(filePtr);
}

template <typename T>
T* readFile(const char* path, int amount = 1) {
    FILE* filePtr = fopen(path, "rb");
    if (filePtr == NULL) showErrorAndExit("Can't open file for reading ='(");

    T* outputPtr = new T[amount];
    int elemsRead = fread(outputPtr, sizeof(T), amount, filePtr);
    if (elemsRead < amount || ferror(filePtr)) showErrorAndExit("File is broken ='(");

    fclose(filePtr);
    return outputPtr;
}