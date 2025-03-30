#include "PrintJobHandler.h"
#include "PrinterManager.h"
#include "FileConverter.h"
#include <iostream>

bool PrintJobHandler::processPrintJob(const std::string &printerName, const std::string &filePath)
{
    // 调用 FileConverter 将文件转换为可打印格式（如 PDF）
    std::string convertedFile;
    if (!FileConverter::convertToPrintableFormat(filePath, convertedFile))
    {
        std::cerr << "File conversion failed for " << filePath << std::endl;
        return false;
    }

    // 调用 PrinterManager 将文件发送给打印机进行打印
    if (!PrinterManager::printFile(printerName, convertedFile))
    {
        std::cerr << "Failed to print file: " << convertedFile << " on printer: " << printerName << std::endl;
        return false;
    }

    std::cout << "Print job for " << convertedFile << " successfully sent to " << printerName << std::endl;
    return true;
}
