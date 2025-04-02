#include "PDFConverter.h"
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
    // Set console code page to UTF-8. Needs compatible font.
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); // If reading input via std::cin potentially
#endif

    std::string inputFilePathStr = "D:\\阿巴阿巴\\调剂.xlsx";

    std::cout << "Attempting to convert file: " << inputFilePathStr << std::endl;

    std::string outputFilePathStr;
    bool success = PDFConverter::convertToPDF(inputFilePathStr, outputFilePathStr);

    if (success)
    {
        std::cout << "\nConversion successful!" << std::endl;
        std::cout << "Output PDF file: " << outputFilePathStr << std::endl;
        return 0;
    }
    else
    {
        std::cout << "\nConversion failed." << std::endl;
        return 1;
    }
}

// g++ -std=c++17 PDFConverter.cpp test.cpp -o pft.exe -static -lstdc++fs