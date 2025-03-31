#include <iostream>
#include "PrintJob.h"

int main()
{
    std::string inputFilePath = "D:/C/printer/files/test.txt";
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
    if (printJob.createPrintJob(inputFilePath, selectedPrinter))
    {
        if (printJob.sendToPrinter())
        {
            if (printJob.monitorPrintStatus())
            {
                std::cout << "Print job completed successfully." << std::endl;
            }
        }
    }
    return 0;
}

// g++ -std=c++17 -I. PrinterManager.cpp PDFConverter.cpp PrintJob.cpp main.cpp -o print_job_app
// g++ -std=c++17 -I. PrinterManager.cpp PDFConverter.cpp PrintJob.cpp main.cpp -lwinspool -o print_job_app