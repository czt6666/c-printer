// PrinterDiscovery.cpp
// cSpell:disable
#include <iostream>
#include <vector>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <winspool.h>
#define UNICODE
#define _UNICODE

#elif defined(__linux__) || defined(__APPLE__)
#include <cups/cups.h>
#endif

#ifdef _WIN32
// 宽字符转换 UTF-8
std::string WideCharToString(const LPCWSTR wstr)
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

// Windows 下获取打印机列表
std::vector<std::string> GetPrinters(DWORD flags)
{
    std::vector<std::string> printers;
    DWORD needed = 0, returned = 0;

    EnumPrintersW(flags, nullptr, 2, nullptr, 0, &needed, &returned);
    if (needed == 0)
        return printers;

    BYTE *buffer = new BYTE[needed];
    if (EnumPrintersW(flags, nullptr, 2, buffer, needed, &needed, &returned))
    {
        PRINTER_INFO_2W *printerInfo = reinterpret_cast<PRINTER_INFO_2W *>(buffer);
        for (DWORD i = 0; i < returned; ++i)
        {
            printers.push_back(WideCharToString(printerInfo[i].pPrinterName));
        }
    }

    delete[] buffer;

    return printers;
}

#elif defined(__linux__) || defined(__APPLE__)
// CUPS（Linux/macOS）下获取打印机列表
std::vector<std::string> GetPrinters(int type)
{
    std::vector<std::string> printers;
    cups_dest_t *dests;
    int num_dests = cupsGetDests(&dests);

    for (int i = 0; i < num_dests; ++i)
    {
        if (type == 0 || (type == 1 && dests[i].is_default))
        {
            printers.push_back(dests[i].name);
        }
    }
    cupsFreeDests(num_dests, dests);
    return printers;
}
#endif

class PrinterDiscovery
{
public:
    static std::vector<std::string> listPrinters()
    {
#ifdef _WIN32
        return GetPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS);
#elif defined(__linux__) || defined(__APPLE__)
        return GetPrinters(0);
#endif
    }

    static std::vector<std::string> listNetworkPrinters()
    {
#ifdef _WIN32
        return GetPrinters(PRINTER_ENUM_NETWORK);
#elif defined(__linux__) || defined(__APPLE__)
        return GetPrinters(1);
#endif
    }
};

int main()
{
    std::cout << "Listing Local Printers:" << std::endl;
    for (const auto &printer : PrinterDiscovery::listPrinters())
    {
        std::cout << "Found printer: " << printer << std::endl;
    }

    std::cout << "\nListing Network Printers:" << std::endl;
    for (const auto &printer : PrinterDiscovery::listNetworkPrinters())
    {
        std::cout << "Found network printer: " << printer << std::endl;
    }
    return 0;
}

// g++ PrinterDiscovery3.cpp -o pd3.exe -I../include -lwinspool
// g++ PrinterDiscovery3.cpp -o pd3 -I../include -lcups