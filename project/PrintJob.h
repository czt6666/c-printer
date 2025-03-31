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
    // 监控打印状态（简单模拟，实际需根据系统 API 实现）
    bool monitorPrintStatus();

private:
    PrinterManager printerManager;
    PDFConverter pdfConverter;
    std::string pdfFilePath;
    std::string selectedPrinter;
};

#endif