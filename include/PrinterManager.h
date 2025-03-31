// PrinterManager.h
#ifndef PRINTER_MANAGER_H
#define PRINTER_MANAGER_H

#include <string>

class PrinterManager
{
public:
    static bool printFile(const std::string &printerName, const std::string &filePath);
};

#endif // PRINTER_MANAGER_H