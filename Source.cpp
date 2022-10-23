#include <iostream>
#include <iomanip>
#include <Windows.h>

using namespace std;

enum Menu { EXIT, WRITE, READ, PRINT, WRITE_TO_FILE };
const int NUMBERS_AMOUNT = 10;

char* computeOutputStr(double*);

int writeNumbersToBuffer(char*, int, double*);

int findMin(double*, int);

void getNumbers(double*, int);

void getMenuFunctionNumber(int*);

void showErrorAndExit(const char*);

void printMessage(const char*, const char*);

void writeMessageToFile(const char*, const char*);

template <typename T>
void writeFile(const char*, T*, int);

template <typename T>
T* readFile(const char*, int);

int main(int argc, char** argv) {
    if (argc == 1) showErrorAndExit("You should specify file path!");
    char* path = argv[1];

    double* numbers = new double[NUMBERS_AMOUNT];
    char* outputStr;

    int functionNumber = 1;
    while (functionNumber && !cin.fail()) {
        getMenuFunctionNumber(&functionNumber);
        switch (functionNumber) {
        case EXIT:
            functionNumber = 0;
            break;
        case WRITE:
            getNumbers(numbers, NUMBERS_AMOUNT);
            writeFile<double>(path, numbers, NUMBERS_AMOUNT);
            break;
        case READ:
        case PRINT:
        case WRITE_TO_FILE:
            numbers = readFile<double>(path, NUMBERS_AMOUNT);
            if (functionNumber == READ) break;
            outputStr = computeOutputStr(numbers);
            if (functionNumber == PRINT) { printMessage(outputStr, "Negative numbers"); }
            else if (functionNumber == WRITE_TO_FILE) writeMessageToFile(outputStr, path);
            delete[] outputStr;
            break;
        default:
            cout << "Invalid function number!" << endl;
            break;
        }
    }

    delete[] numbers;
    return 0;
}

char* computeOutputStr(double* numbers) {
    int bufferLength = NUMBERS_AMOUNT * 10;
    char* outputBuffer = new char[bufferLength];
    writeNumbersToBuffer(outputBuffer, bufferLength, numbers);
    return outputBuffer;
}

int writeNumbersToBuffer(char* buffer, int bufferLength, double* numbers) {
    int minIndex = findMin(numbers, NUMBERS_AMOUNT);
    int bufferPos = 0;
    for (int i = 0; i < minIndex; i++) {
        if (bufferPos >= bufferLength) {
            printMessage("Some numbers could be missing!!!", "Buffer overflow");
            return 1;
        }
        if (numbers[i] < 0) {
            bufferPos += sprintf(buffer + bufferPos, "%.2g ", numbers[i]);
        }
    }
    if (!bufferPos) sprintf(buffer, "%s", "Not found ='(");
    return 0;
}

int findMin(double* numbers, int length) {
    int minIndex = 0;
    for (int i = 1; i < length; i++)
        if (numbers[i] < numbers[minIndex]) minIndex = i;
    return minIndex;
}

void getNumbers(double* outputArr, int amount) {
    cout << "Enter numbers (separated by space or new line): ";
    for (int i = 0; i < amount; i++) {
        cin >> outputArr[i];
    }
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