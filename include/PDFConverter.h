#ifndef PDFCONVERTER_H
#define PDFCONVERTER_H

#include <string>

class PDFConverter
{
public:
    // 将文件转换为 PDF
    bool convertToPDF(const std::string &inputPath, std::string &outputPath);
};

#endif