// StringConverter.h
#ifndef STRING_CONVERTER_H
#define STRING_CONVERTER_H

#include <windows.h>
#include <string>

std::wstring Utf8ToWString(const std::string &str);
std::string WStringToUtf8(const std::wstring &wstr);

#endif // STRING_CONVERTER_H