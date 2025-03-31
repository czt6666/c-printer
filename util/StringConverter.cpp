#include "StringConverter.h"

#include <windows.h>
#include <string>
std::wstring Utf8ToWString(const std::string &str)
{
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    std::wstring wideStr(wideLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wideStr[0], wideLen);
    return wideStr;
}

std::string WStringToUtf8(const std::wstring &wstr)
{
    int ansiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string ansiStr(ansiLen, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &ansiStr[0], ansiLen, NULL, NULL);
    return ansiStr;
}