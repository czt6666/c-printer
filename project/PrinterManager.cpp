#include "PrinterManager.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#include <winspool.h>
#define UNICODE
#define _UNICODE
#elif defined(__linux__) || defined(__APPLE__)
#include <cups/cups.h>
#endif

// 宽字符转换 UTF-8
std::string WideCharToString(const wchar_t *wstr)
{
    if (!wstr)
        return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0)
        return "";
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], size_needed, nullptr, nullptr);
    str.resize(size_needed - 1);
    return str;
}

std::vector<std::string> PrinterManager::listAvailablePrinters()
{
    std::vector<std::string> printers;
#ifdef _WIN32
    DWORD needed = 0, returned = 0;
    EnumPrintersW(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, nullptr, 0, &needed, &returned);
    if (needed == 0)
        return printers;
    BYTE *buffer = new BYTE[needed];
    if (EnumPrintersW(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, buffer, needed, &needed, &returned))
    {
        PRINTER_INFO_2W *printerInfo = reinterpret_cast<PRINTER_INFO_2W *>(buffer);
        for (DWORD i = 0; i < returned; ++i)
        {
            printers.push_back(WideCharToString(printerInfo[i].pPrinterName));
        }
    }
    delete[] buffer;
#elif defined(__linux__) || defined(__APPLE__)
    cups_dest_t *dests;
    int num_dests = cupsGetDests(&dests);
    for (int i = 0; i < num_dests; ++i)
    {
        printers.push_back(dests[i].name);
    }
    cupsFreeDests(num_dests, dests);
#endif
    return printers;
}

bool PrinterManager::selectPrinter(const std::string &printerName)
{
    auto printers = listAvailablePrinters();
    if (std::find(printers.begin(), printers.end(), printerName) != printers.end())
    {
        selectedPrinter = printerName;
        return true;
    }
    return false;
}

std::string PrinterManager::getSelectedPrinter()
{
    return selectedPrinter;
}