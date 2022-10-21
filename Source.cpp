#include <iostream>
#include <Windows.h>

using namespace std;

void printMessage(char*, char*);

void writeMessageToFile(char*, char*);

void showErrorAndExit(const char*);

template <typename T, int N>
void createFile(char*, T*);

template <typename T, int N>
T* readFile(char*);

int main() {
    printMessage(const_cast<char*>(""), const_cast <char*>(""));
    writeMessageToFile(const_cast <char*>(""), const_cast <char*>(""));

    return 0;
}

void printMessage(char* message, char* windowTitle) {
    MessageBoxA(NULL, message, windowTitle, MB_OK);
}

void writeMessageToFile(char* message, char* path) {
    FILE* filePtr = fopen(path, "w");
    if (filePtr == NULL) showErrorAndExit("Can't create file ='(");

    fputs(message, filePtr);
    if (ferror(filePtr)) showErrorAndExit("Error while writing file ='(");

    fclose(filePtr);
}

void showErrorAndExit(const char* errorMessage) {
    _fcloseall();
    cout << errorMessage << endl;
    exit(1);
}

template <typename T, int N = 1>
void createFile(char* path, T* dataForWriting) {
    FILE* filePtr = fopen(path, "wb");
    if (filePtr == NULL) showErrorAndExit("Can't create file ='(");

    fwrite(dataForWriting, sizeof(T), N, filePtr);
    if (ferror(filePtr)) showErrorAndExit("Error while writing file ='(");

    fclose(filePtr);
}

template <typename T, int N>
T* readFile(char* path) {
    FILE* filePtr = fopen(path, "rb");
    if (filePtr == NULL) showErrorAndExit("Can't open file for reading ='(");

    T* outputPtr;
    int elemsRead = fread(outputPtr, sizeof(T), N, filePtr);
    if (elemsRead < N || ferror(filePtr)) showErrorAndExit("File is broken ='(");

    fclose(filePtr);
    return outputPtr;
}