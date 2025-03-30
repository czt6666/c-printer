// PrinterDiscovery.h
#ifndef PRINTER_DISCOVERY_H
#define PRINTER_DISCOVERY_H

#include <vector>
#include <string>

class PrinterDiscovery
{
public:
    static std::vector<std::string> listPrinters();
    static std::vector<std::string> listNetworkPrinters();
};

#endif // PRINTER_DISCOVERY_H