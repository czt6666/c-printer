#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#include <string>

class FileConverter
{
public:
    bool convertToPrintableFormat(const std::string &inputPath, std::string &outputPath);
};

#endif