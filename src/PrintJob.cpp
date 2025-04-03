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
    command = powerShellPath + " -Command \"Start-Process -FilePath '" + sumatraPDFPath + "' -ArgumentList '-print-to \\\"" + selectedPrinter + "\\\" \\\"" + pdfFilePath + "\\\"' -NoNewWindow\"";
#elif defined(__linux__) || defined(__APPLE__)
    command = "lp -d \"" + selectedPrinter + "\" \"" + pdfFilePath + "\"";
#endif
    return executeCommand(command);
}
