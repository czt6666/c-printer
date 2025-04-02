#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <functional>
#include <cstdlib>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
void setConsoleColor(WORD color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
#define COLOR_RED setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define COLOR_GREEN setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define COLOR_RESET setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#else
#define COLOR_RED std::cerr << "\033[31m"
#define COLOR_GREEN std::cout << "\033[32m"
#define COLOR_RESET std::cerr << "\033[0m"
#endif

void printError(const std::string &message)
{
    COLOR_RED;
    std::cerr << "Error: " << message << std::endl;
    COLOR_RESET;
}

bool executeCommand(const std::string &command)
{
    COLOR_GREEN;
    std::cout << "Executing: " << command << std::endl;
    COLOR_RESET;
    int result = std::system(command.c_str());
    return result == 0;
}

std::string getFileHash(const std::string &inputPath)
{
    std::ifstream file(inputPath, std::ios::binary);
    if (!file)
    {
        printError("Failed to open file for hashing: " + inputPath);
        return "";
    }
    std::hash<std::string> hash_fn;
    size_t hash = 0;
    const size_t bufferSize = 4096;
    char buffer[bufferSize];
    while (file.read(buffer, bufferSize))
    {
        std::string chunk(buffer, bufferSize);
        hash ^= hash_fn(chunk) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    if (file.gcount() > 0)
    {
        std::string chunk(buffer, file.gcount());
        hash ^= hash_fn(chunk) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    std::stringstream hashStream;
    hashStream << std::hex << std::setw(16) << std::setfill('0') << hash;
    return hashStream.str();
}

std::string generateConvertedFileName(const std::string &inputPath)
{
    std::filesystem::path inputFilePath(inputPath);
    std::string fileHash = getFileHash(inputPath);
    if (fileHash.empty())
    {
        printError("Failed to get file hash for: " + inputPath);
        return "";
    }
    return inputFilePath.stem().string() + "-" + fileHash + ".pdf";
}

#ifdef _WIN32
std::string wstring_to_utf8(const std::wstring &wstr)
{
    if (wstr.empty())
        return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    if (size_needed <= 0)
    {
        std::cerr << "WideCharToMultiByte failed to get size. Error code: " << GetLastError() << std::endl;
        return "";
    }
    std::string strTo(size_needed, 0);
    int bytes_converted = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    if (bytes_converted <= 0)
    {
        std::cerr << "WideCharToMultiByte failed to convert. Error code: " << GetLastError() << std::endl;
        return "";
    }
    return strTo;
}

std::wstring utf8_to_wstring(const std::string &str)
{
    if (str.empty())
        return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    if (size_needed <= 0)
    {
        std::cerr << "MultiByteToWideChar failed to get size. Error code: " << GetLastError() << std::endl;
        return L"";
    }
    std::wstring wstrTo(size_needed, 0);
    int chars_converted = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    if (chars_converted <= 0)
    {
        std::cerr << "MultiByteToWideChar failed to convert. Error code: " << GetLastError() << std::endl;
        return L"";
    }
    return wstrTo;
}

#endif