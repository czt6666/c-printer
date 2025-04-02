#include "PrintJob.h"
#include "PDFConverter.h"
#include "Utils.h"
#include <iostream>
#include <cstdlib>

#ifdef _WIN32
const std::string sumatraPDFPath = "C:\\Program Files\\SumatraPDF\\SumatraPDF.exe";
const std::string powerShellPath = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
#endif

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
    // C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe
    //  -Command "Start-Process -FilePath
    // 'C:\Program Files\SumatraPDF\SumatraPDF.exe'
    //  -ArgumentList '-print-to
    // \"Hewlett-Packard HP LaserJet 500 color M551\"
    // \"D:\C\printer\files\test-e11bda1226f08065.pdf\"'
    //  -NoNewWindow"
    command = powerShellPath + " -Command \"Start-Process -FilePath '" + sumatraPDFPath + "' -ArgumentList '-print-to \\\"" + selectedPrinter + "\\\" \\\"" + pdfFilePath + "\\\"' -NoNewWindow\"";
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
