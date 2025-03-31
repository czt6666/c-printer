// cSpell:disable
#include "PrinterDiscovery.h"
#include "../include/OSAdapter.h"
#include "PrintJobHandler.h"
#include <string>
#include <iostream>

// Test function
int main()
{
    std::string testPrinter = OSAdapter::getDefaultPrinter();
    std::string testFile = "sample.txt";

    if (testPrinter.empty())
    {
        std::cout << "No default printer found." << std::endl;
        return 1;
    }

    if (PrintJobHandler::processPrintJob(testPrinter, testFile))
    {
        std::cout << "Print job sent successfully." << std::endl;
    }
    else
    {
        std::cout << "Print job failed." << std::endl;
    }

    return 0;
}

// mac or linux
// g++ -o printer_app main.cpp PrinterDiscovery.cpp PrinterManager.cpp FileConverter.cpp PrintJobHandler.cpp OSAdapter.cpp -lcups
// ./printer_app

// windows
// g++ -o printer_app main.cpp PrinterDiscovery.cpp PrinterManager.cpp FileConverter.cpp PrintJobHandler.cpp OSAdapter.cpp -lwinspool
// printer_app.exe

// g++ -I../include -lwinspool -windows main.cpp
