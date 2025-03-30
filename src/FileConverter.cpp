#include "FileConverter.h"
#include <iostream>
#include <filesystem>
#include <cstdlib> // 用于调用外部工具

#ifdef _WIN32
#define SYSTEM_CONVERT_COMMAND "soffice --headless --convert-to pdf"
#elif defined(__linux__) || defined(__APPLE__)
#define SYSTEM_CONVERT_COMMAND "libreoffice --headless --convert-to pdf"
#endif

bool FileConverter::convertToPrintableFormat(const std::string &inputPath, std::string &outputPath)
{
    // 获取输入文件的扩展名
    std::filesystem::path inputFilePath(inputPath);
    std::string extension = inputFilePath.extension().string();

    // 仅支持已知的可转换格式
    if (extension != ".txt" && extension != ".docx" && extension != ".png" && extension != ".jpg" && extension != ".odt")
    {
        std::cerr << "Unsupported file format: " << extension << std::endl;
        return false;
    }

    // 生成输出文件路径（替换扩展名）
    outputPath = inputFilePath.replace_extension(".pdf").string();

    // 调用系统转换工具
    std::string command = SYSTEM_CONVERT_COMMAND + std::string(" \"") + inputPath + "\" --outdir \"" + inputFilePath.parent_path().string() + "\"";

    std::cout << "Executing: " << command << std::endl;
    int result = std::system(command.c_str());

    if (result != 0)
    {
        std::cerr << "File conversion failed for " << inputPath << std::endl;
        return false;
    }

    std::cout << "Converted: " << inputPath << " -> " << outputPath << std::endl;
    return true;
}
