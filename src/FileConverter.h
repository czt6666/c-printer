#ifndef FILE_CONVERTER_H
#define FILE_CONVERTER_H

#include <string>

class FileConverter
{
public:
    // 将输入文件转换为可打印格式（例如 PDF）
    static bool convertToPrintableFormat(const std::string &inputPath, std::string &outputPath);
};

#endif // FILE_CONVERTER_H
