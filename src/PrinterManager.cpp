// PrinterManager.cpp
#include "PrinterManager.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <winspool.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <cups/cups.h>
#endif

bool PrinterManager::printFile(const std::string &printerName, const std::string &filePath)
{
#ifdef _WIN32
    DOC_INFO_1 docInfo;
    docInfo.pDocName = "C++ Print Job";
    docInfo.pOutputFile = nullptr;
    docInfo.pDatatype = "RAW";

    HANDLE hPrinter;
    if (!OpenPrinter(const_cast<char *>(printerName.c_str()), &hPrinter, nullptr))
    {
        std::cerr << "Failed to open printer: " << printerName << std::endl;
        return false;
    }

    if (!StartDocPrinter(hPrinter, 1, (LPBYTE)&docInfo))
    {
        ClosePrinter(hPrinter);
        std::cerr << "Failed to start document on printer: " << printerName << std::endl;
        return false;
    }

    EndDocPrinter(hPrinter);
    ClosePrinter(hPrinter);
    return true;

#elif defined(__linux__) || defined(__APPLE__)
    int job_id = cupsPrintFile(printerName.c_str(), filePath.c_str(), "C++ Print Job", 0, nullptr);
    if (job_id == 0)
    {
        std::cerr << "Failed to print file: " << filePath << " on printer: " << printerName << std::endl;
        return false;
    }
    return true;
#endif
}