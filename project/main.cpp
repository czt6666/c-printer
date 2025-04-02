#include <iostream>
#include "PrintJob.h"
#include "FileSelecter.h"

int main()
{
    long long maxFileSize = 1024 * 1024 * 10;
    // std::string inputFilePath = handleFileSelection(maxFileSize);
    std::string inputFilePath = "D:\\阿巴阿巴\\调剂.xlsx";
    if (inputFilePath.empty())
    {
        std::cout << "No valid file was selected." << std::endl;
        return 1;
    }
    std::cout << "The selected file path is: " << inputFilePath << std::endl;

    PrintJob printJob;
    PrinterManager printerManager;
    auto printers = printerManager.listAvailablePrinters();
    if (printers.empty())
    {
        std::cerr << "No available printers found." << std::endl;
        return 1;
    }
    std::cout << "Available printers:" << std::endl;
    for (size_t i = 0; i < printers.size(); ++i)
    {
        std::cout << i + 1 << ". " << printers[i] << std::endl;
    }
    int choice;
    std::cout << "Select a printer (enter number): ";
    std::cin >> choice;
    if (choice < 1 || choice > static_cast<int>(printers.size()))
    {
        std::cerr << "Invalid choice." << std::endl;
        return 1;
    }
    std::string selectedPrinter = printers[choice - 1];
    if (!printJob.createPrintJob(inputFilePath, selectedPrinter))
    {
        std::cerr << "Error creating print job." << std::endl;
        return 1;
    }
    if (!printJob.sendToPrinter())
    {
        std::cerr << "Error sending print job to printer." << std::endl;
        return 1;
    }
    // if (!printJob.monitorPrintStatus())
    // {
    //     std::cerr << "Error monitoring print status." << std::endl;
    //     return 0;
    // }
    std::cout << "Print job completed successfully." << std::endl;
    return 0;
}

// g++ -std=c++17 -I. Utils.cpp FileSelecter.cpp PrinterManager.cpp PDFConverter.cpp PrintJob.cpp main.cpp -lcups -o pja
// g++ -std=c++17 -I. Utils.cpp FileSelecter.cpp PrinterManager.cpp PDFConverter.cpp PrintJob.cpp main.cpp -lwinspool -lcomdlg32 -o pja