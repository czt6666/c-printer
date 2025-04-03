#ifndef PRINTERMANAGER_H
#define PRINTERMANAGER_H

#include <vector>
#include <string>

class PrinterManager
{
public:
    // 列出可用的打印机
    std::vector<std::string> listAvailablePrinters();
    // 选择打印机
    bool selectPrinter(const std::string &printerName);
    // 获取当前选择的打印机
    std::string getSelectedPrinter();

private:
    std::string selectedPrinter;
};

#endif