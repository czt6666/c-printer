#include "printer_manager.h"
#include "file_converter.h"
#include <iostream>
#include <string>

int main()
{
    PrinterManager printerManager;
    FileConverterManager converterManager;

    // 列出打印机列表
    std::vector<std::string> printerList = printerManager.getPrinterList();
    std::cout << "Available printers:" << std::endl;
    for (const auto &printerName : printerList)
    {
        std::cout << "- " << printerName << std::endl;
    }

    // 选择打印机
    std::string selectedPrinterName;
    std::cout << "Enter the name of the printer you want to use: ";
    std::getline(std::cin, selectedPrinterName);
    Printer *selectedPrinter = printerManager.selectPrinter(selectedPrinterName);

    // 选择打印文件
    std::string inputFilePath;
    std::cout << "Enter the path of the file you want to print: ";
    std::getline(std::cin, inputFilePath);

    // 转换文件格式
    std::string convertedFilePath = converterManager.convertFile(inputFilePath);

    // 打印文件
    printerManager.printFile(selectedPrinter, convertedFilePath);

    return 0;
}