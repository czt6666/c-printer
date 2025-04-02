#include "PrintJob.h"
#include "PDFConverter.h"
#include <iostream>
#include <cstdlib>

bool PrintJob::createPrintJob(const std::string &inputFilePath, const std::string &printerName)
{
    if (!printerManager.selectPrinter(printerName))
    {
        std::cerr << "Invalid printer selection." << std::endl;
        return false;
    }
    selectedPrinter = printerManager.getSelectedPrinter();
    if (!pdfConverter.convertToPDF(inputFilePath, pdfFilePath))
    {
        std::cerr << "File conversion to PDF failed." << std::endl;
        return false;
    }
    return true;
}

bool PrintJob::sendToPrinter()
{
    std::string command;
#ifdef _WIN32
    // print /d:"Hewlett-Packard HP LaserJet 500 color M551" "D:/C/printer/files/test-e11bda1226f08065.pdf"
    // start microsoft-edge:https://localhost/print?file="D:/C/printer/files/test-e11bda1226f08065.pdf"
    command = "rundll32.exe printui.dll,PrintUIEntry /c /n \"" + selectedPrinter + "\" /pt /f \"" + pdfFilePath + "\"";
#elif defined(__linux__) || defined(__APPLE__)
    command = "lp -d \"" + selectedPrinter + "\" \"" + pdfFilePath + "\"";
#endif
    return executeCommand(command);
}

bool PrintJob::monitorPrintStatus()
{
    // 简单模拟，实际需要根据系统 API 实现
    std::cout << "Print status monitoring is not fully implemented yet. Assume success for now." << std::endl;
    return true;
}

// $printerName = "Hewlett-Packard HP LaserJet 500 color M551"
// $pdfPath = "D:/C/printer/files/test-e11bda1226f08065.pdf"
// Set-ItemProperty -Path "HKCU:\Software\Microsoft\Windows NT\CurrentVersion\Windows" -Name "Device" -Value "\$printerName,winspool,Ne00:"
// Start-Process -FilePath \$pdfPath -Verb Print
