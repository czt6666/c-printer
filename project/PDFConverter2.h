#ifndef PDFCONVERTER_H
#define PDFCONVERTER_H

#include <string>

class PDFConverter
{
public:
    // Converts the given input file to PDF.
    // inputPath: UTF-8 encoded path to the input file.
    // outputPath: UTF-8 encoded path to the generated PDF file if successful.
    // Returns true on success, false on failure.
    static bool convertToPDF(const std::string &inputPath, std::string &outputPath);
};

#endif // PDFCONVERTER_H