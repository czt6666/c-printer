// PrinterDiscovery.cpp
// cSpell:disable
#include "PrinterDiscovery.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <winspool.h>
#define UNICODE
#define _UNICODE

#elif defined(__linux__) || defined(__APPLE__)
#include <cups/cups.h>
#endif

// 转换宽字符为 UTF-8 字符串
std::string WideCharToString(const LPCWSTR wstr)
{
    if (!wstr)
        return "";

    int size_needed = WideCharToMultiByte(
        CP_UTF8, 0,
        wstr, -1,
        nullptr, 0,
        nullptr, nullptr);
    if (size_needed == 0)
        return "";

    // 执行转换
    std::string str(size_needed, 0);
    WideCharToMultiByte(
        CP_UTF8, 0,
        wstr, -1,
        &str[0], size_needed,
        nullptr, nullptr);

    // 移除末尾的空字符
    str.resize(size_needed - 1);
    return str;
}

std::vector<std::string> PrinterDiscovery::listPrinters()
{
    std::vector<std::string> printers;

#ifdef _WIN32
    DWORD needed = 0, returned = 0;

    // 第一次调用获取所需缓冲区大小
    EnumPrinters(
        PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS,
        nullptr,
        2,
        nullptr,
        0,
        &needed,
        &returned);

    if (needed == 0)
        return printers;

    BYTE *buffer = new BYTE[needed];

    // 第二次调用获取实际数据
    if (EnumPrinters(
            PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS,
            nullptr,
            2,
            buffer,
            needed,
            &needed,
            &returned))
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

std::vector<std::string> PrinterDiscovery::listNetworkPrinters()
{
    std::vector<std::string> networkPrinters;

#ifdef _WIN32
    DWORD needed = 0, returned = 0;

    EnumPrintersW(
        PRINTER_ENUM_NETWORK,
        nullptr,
        2,
        nullptr,
        0,
        &needed,
        &returned);

    if (needed == 0)
        return networkPrinters;

    BYTE *buffer = new BYTE[needed];

    if (EnumPrintersW(
            PRINTER_ENUM_NETWORK,
            nullptr,
            2,
            buffer,
            needed,
            &needed,
            &returned))
    {
        PRINTER_INFO_2W *printerInfo = reinterpret_cast<PRINTER_INFO_2W *>(buffer);
        for (DWORD i = 0; i < returned; ++i)
        {
            networkPrinters.push_back(WideCharToString(printerInfo[i].pPrinterName));
        }
    }

    delete[] buffer;

#elif defined(__linux__) || defined(__APPLE__)
    cups_dest_t *dests;
    int num_dests = cupsGetDests(&dests);
    for (int i = 0; i < num_dests; ++i)
    {
        if (dests[i].is_default)
        {
            networkPrinters.push_back(dests[i].name);
        }
    }
    cupsFreeDests(num_dests, dests);
#endif

    return networkPrinters;
}

int main()
{
    std::cout << "Listing Local Printers:" << std::endl;
    auto printers = PrinterDiscovery::listPrinters();
    for (const auto &printer : printers)
    {
        std::cout << "Found printer: " << printer << std::endl;
    }

    std::cout << "\nListing Network Printers:" << std::endl;
    auto networkPrinters = PrinterDiscovery::listNetworkPrinters();
    for (const auto &printer : networkPrinters)
    {
        std::cout << "Found network printer: " << printer << std::endl;
    }

    return 0;
}

// g++ PrinterDiscovery2.cpp -o pd.exe -I../include -lwinspool
