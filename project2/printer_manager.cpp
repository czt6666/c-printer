#ifndef PRINTER_MANAGER_H
#define PRINTER_MANAGER_H
#include <vector>
#include <string>
// 打印机基类
class Printer
{
public:
    virtual ~Printer() = default;
    virtual void print(const std::string &filePath) = 0;
    virtual std::string getName() const = 0;
};
// 打印机管理器类
class PrinterManager
{
public:
    PrinterManager();
    ~PrinterManager();
    std::vector<std::string> getPrinterList();
    Printer *selectPrinter(int printerIndex);
    void printFile(Printer *printer, const std::string &filePath);

private:
    std::vector<Printer *> printers;
    void initializePrinters();
};
#endif // PRINTER_MANAGER_H
#include "printer_manager.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#elif __APPLE__ || __linux__
#include <cups/cups.h>
#endif
// Windows 下的打印机实现类
class WindowsPrinter : public Printer
{
public:
    WindowsPrinter(const std::wstring &printerName) : printerNameW(printerName)
    {
        printerNameA = convertWStringToAString(printerName);
    }
    void print(const std::string &filePath) override
    {
        std::wstring filePathW = convertAStringToWString(filePath);
        ShellExecuteW(nullptr, L"print", filePathW.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    }
    std::string getName() const override
    {
        return printerNameA;
    }

private:
    std::wstring printerNameW;
    std::string printerNameA;
    std::wstring convertAStringToWString(const std::string &str)
    {
        int len;
        int slength = static_cast<int>(str.length()) + 1;
        len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
        std::wstring r(len, L'\0');
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, &r[0], len);
        return r;
    }
    std::string convertWStringToAString(const std::wstring &wstr)
    {
        int len;
        int slength = static_cast<int>(wstr.length()) + 1;
        len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, 0, 0, 0, 0);
        std::string r(len, '\0');
        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, &r[0], len, 0, 0);
        return r;
    }
};
// __linux__ 和 macOS 下的打印机实现类
class UnixPrinter : public Printer
{
public:
    UnixPrinter(const std::string &printerName) : printerName(printerName) {}
    void print(const std::string &filePath) override
    {
        std::string command = "lp -d" + printerName + " " + filePath;
        system(command.c_str());
    }
    std::string getName() const override
    {
        return printerName;
    }

private:
    std::string printerName;
};
// 打印机管理器构造函数
PrinterManager::PrinterManager()
{
    initializePrinters();
}
// 打印机管理器析构函数
PrinterManager::~PrinterManager()
{
    for (auto printer : printers)
    {
        delete printer;
    }
}
// 初始化打印机列表
void PrinterManager::initializePrinters()
{
#ifdef _WIN32
    DWORD needed, returned;
    EnumPrintersW(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 4, nullptr, 0, &needed, &returned);
    std::vector<BYTE> buffer(needed);
    if (EnumPrintersW(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 4, buffer.data(), needed, &needed, &returned))
    {
        PRINTER_INFO_4W *printersInfo = reinterpret_cast<PRINTER_INFO_4W *>(buffer.data());
        for (DWORD i = 0; i < returned; ++i)
        {
            printers.push_back(new WindowsPrinter(printersInfo[i].pPrinterName));
        }
    }
#elif __APPLE__ || __linux__
    cups_dest_t *dests;
    int num_dests = cupsGetDests(&dests);
    for (int i = 0; i < num_dests; ++i)
    {
        printers.push_back(new UnixPrinter(dests[i].name));
    }
    cupsFreeDests(num_dests, dests);
#else
    std::cerr << "Unsupported operating system." << std::endl;
#endif
}
// 获取打印机列表
std::vector<std::string> PrinterManager::getPrinterList()
{
    std::vector<std::string> printerNames;
    for (auto printer : printers)
    {
        printerNames.push_back(printer->getName());
    }
    return printerNames;
}
// 选择打印机
Printer *PrinterManager::selectPrinter(int printerIndex)
{
    if (printerIndex >= 0 && printerIndex < static_cast<int>(printers.size()))
    {
        return printers[printerIndex];
    }
    return nullptr;
}
// 打印文件
void PrinterManager::printFile(Printer *printer, const std::string &filePath)
{
    if (printer)
    {
        printer->print(filePath);
    }
    else
    {
        std::cerr << "Invalid printer selection." << std::endl;
    }
}