// PrinterDiscovery.cpp
#include "PrinterDiscovery.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <winspool.h>
#endif

// 转换宽字符为 UTF-8 字符串
std::string WideCharToString(const LPWSTR wstr)
{
    // 获取转换所需的缓冲区大小
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    if (size_needed == 0)
    {
        return "";
    }
    std::string str(size_needed, 0);
    // 进行实际的转换
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], size_needed, nullptr, nullptr);
    // 去掉末尾的空字符
    str.resize(str.size() - 1);
    return str;
}

bool IsWideString(LPVOID str)
{
    // 这是一个简单的启发式方法，不保证100%准确
    // 更好的方法是在设计时跟踪字符串类型
    wchar_t *wstr = static_cast<wchar_t *>(str);
    return wstr[0] < 256 && wstr[1] == 0;
}

std::string SafeWideCharToString(const LPVOID str)
{
    bool isWide = IsWideString(str);
    if (!isWide)
    {
        LPSTR astr = static_cast<LPSTR>(str);
        return astr;
    }

    LPWSTR wstr = static_cast<LPWSTR>(str);
    int size = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    std::string converted(size, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, &converted[0], size, NULL, NULL);
    return converted;
}

void AddToStringVector(std::vector<std::string> &vec, LPVOID str, bool isWide)
{
    if (isWide)
    {
        // 宽字符转换 (LPWSTR -> std::string)
        LPWSTR wstr = static_cast<LPWSTR>(str);
        int size = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
        std::string converted(size, 0);
        WideCharToMultiByte(CP_ACP, 0, wstr, -1, &converted[0], size, NULL, NULL);
        vec.push_back(converted);
    }
    else
    {
        // 直接转换 (LPSTR -> std::string)
        LPSTR astr = static_cast<LPSTR>(str);
        vec.push_back(astr);
    }
}

std::vector<std::string> PrinterDiscovery::listPrinters()
{
    std::vector<std::string> printers;

#ifdef _WIN32
    DWORD needed, returned;
    EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, nullptr, 0, &needed, &returned);
    BYTE *buffer = new BYTE[needed];

    if (EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, buffer, needed, &needed, &returned))
    {
        PRINTER_INFO_2 *printerInfo = (PRINTER_INFO_2 *)buffer;
        for (DWORD i = 0; i < returned; i++)
        {
            std::string printerName = SafeWideCharToString(printerInfo[i].pPrinterName);
            printers.push_back(printerName);
        }
    }
    delete[] buffer;

#elif defined(__linux__) || defined(__APPLE__)
    cups_dest_t *dests;
    int num_dests = cupsGetDests(&dests);
    for (int i = 0; i < num_dests; i++)
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
    DWORD needed, returned;
    EnumPrinters(PRINTER_ENUM_NETWORK, nullptr, 2, nullptr, 0, &needed, &returned);
    BYTE *buffer = new BYTE[needed];
    if (EnumPrinters(PRINTER_ENUM_NETWORK, nullptr, 2, buffer, needed, &needed, &returned))
    {
        PRINTER_INFO_2 *printerInfo = (PRINTER_INFO_2 *)buffer;
        for (DWORD i = 0; i < returned; i++)
        {
            std::string printerName = SafeWideCharToString(printerInfo[i].pPrinterName);
            networkPrinters.push_back(printerName);
        }
    }
    delete[] buffer;
#elif defined(__linux__) || defined(__APPLE__)
    cups_dest_t *dests;
    int num_dests = cupsGetDests(&dests);
    for (int i = 0; i < num_dests; i++)
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