#ifndef PRINTJOB_H
#define PRINTJOB_H

#include <string>
#include "PrinterManager.h"
#include "PDFConverter.h"

class PrintJob
{
public:
    // 创建打印任务
    bool createPrintJob(const std::string &inputFilePath, const std::string &printerName);
    // 发送 PDF 文件到选定的打印机
    bool sendToPrinter();

private:
    PrinterManager printerManager;
    PDFConverter pdfConverter;
    std::string pdfFilePath;
    std::string selectedPrinter;
};

#endif