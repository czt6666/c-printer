#include "OSAdapter.h"
#include "PrinterDiscovery.h"
#include <iostream>

std::string OSAdapter::getDefaultPrinter()
{
    // 获取本地打印机列表
    std::vector<std::string> printers = PrinterDiscovery::listPrinters();

    // 如果打印机列表非空，返回第一个打印机名称作为默认打印机
    if (!printers.empty())
    {
        std::cout << "Default printer: " << printers[0] << std::endl;
        return printers[0];
    }

    // 如果没有找到打印机，返回空字符串
    std::cout << "No printers found." << std::endl;
    return "";
}
