#ifndef PDFCONVERTER_H
#define PDFCONVERTER_H

#include <string>

bool executeCommand(const std::string &command);

class PDFConverter
{
public:
    // 将文件转换为 PDF
    bool convertToPDF(const std::string &inputPath, std::string &outputPath);
};

#endif