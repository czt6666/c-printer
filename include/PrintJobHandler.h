// PrintJobHandler.h
#ifndef PRINT_JOB_HANDLER_H
#define PRINT_JOB_HANDLER_H

#include <string>

class PrintJobHandler
{
public:
    static bool processPrintJob(const std::string &printerName, const std::string &filePath);
};

#endif // PRINT_JOB_HANDLER_H