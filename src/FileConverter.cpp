#include "FileConverter.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib> // 用于调用外部工具
#include <fstream> // 用于文件检查
#include <functional>

#ifdef _WIN32
#define SYSTEM_CONVERT_COMMAND "soffice --headless --convert-to pdf"
#elif defined(__linux__) || defined(__APPLE__)
#define SYSTEM_CONVERT_COMMAND "libreoffice --headless --convert-to pdf"
#endif

// 计算文件的哈希值
std::string getFileHash(const std::string &inputPath)
{
    std::ifstream file(inputPath, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open file for hashing: " << inputPath << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf(); // 读取文件内容
    std::string fileContent = ss.str();

    // 使用 std::hash 计算文件内容的哈希值
    std::hash<std::string> hash_fn;
    size_t hash = hash_fn(fileContent);

    // 将哈希值转换为十六进制字符串
    std::stringstream hashStream;
    hashStream << std::hex << std::setw(16) << std::setfill('0') << hash;
    return hashStream.str();
}

// 生成转换后的文件名，添加哈希值
std::string generateConvertedFileName(const std::string &inputPath)
{
    std::filesystem::path inputFilePath(inputPath);

    // 获取文件名的哈希值
    std::string fileHash = getFileHash(inputPath);
    if (fileHash.empty())
    {
        return ""; // 如果哈希为空，则返回空
    }

    // 自定义文件名生成：保留原文件名，添加哈希值
    std::string customFileName = inputFilePath.stem().string() + "-" + fileHash + ".pdf";

    return customFileName;
}

// 创建临时文件夹
std::filesystem::path createTempPdfDir(const std::filesystem::path &inputFilePath)
{
    std::filesystem::path tempPdfDir = inputFilePath.parent_path() / "tempPdfFiles";
    if (!std::filesystem::exists(tempPdfDir))
    {
        std::filesystem::create_directory(tempPdfDir);
    }
    return tempPdfDir;
}

// 执行转换命令
bool executeConvertCommand(const std::string &inputPath, const std::filesystem::path &tempPdfDir)
{
    std::string convertCommand = SYSTEM_CONVERT_COMMAND;
    convertCommand += " \"" + inputPath + "\" --outdir \"" + tempPdfDir.string() + "\"";

    std::cout << "Executing conversion command: " << std::endl
              << convertCommand << std::endl
              << std::endl;

    int convertResult = std::system(convertCommand.c_str());
    return convertResult == 0;
}

// 重命名并移动文件
bool renameAndMoveFile(const std::filesystem::path &defaultOutputPath, const std::filesystem::path &targetPath)
{
    std::string renameAndMoveCommand;
#ifdef _WIN32
    renameAndMoveCommand = "move \"" + defaultOutputPath.string() + "\" \"" + targetPath.string() + "\"";
#else
    renameAndMoveCommand = "mv \"" + defaultOutputPath.string() + "\" \"" + targetPath.string() + "\"";
#endif

    std::cout << "Executing rename and move command: " << std::endl
              << renameAndMoveCommand << std::endl
              << std::endl;

    int renameAndMoveResult = std::system(renameAndMoveCommand.c_str());
    return renameAndMoveResult == 0;
}

// 删除临时文件夹
bool deleteTempDir(const std::filesystem::path &tempPdfDir)
{
    std::string deleteTempDirCommand;
#ifdef _WIN32
    deleteTempDirCommand = "rmdir /s /q \"" + tempPdfDir.string() + "\"";
#else
    deleteTempDirCommand = "rm -r \"" + tempPdfDir.string() + "\"";
#endif

    std::cout << "Executing delete temp directory command: " << std::endl
              << deleteTempDirCommand << std::endl
              << std::endl;

    int deleteTempDirResult = std::system(deleteTempDirCommand.c_str());
    return deleteTempDirResult == 0;
}

bool FileConverter::convertToPrintableFormat(const std::string &inputPath, std::string &outputPath)
{
    std::filesystem::path inputFilePath(inputPath);

    // 1. **检查文件是否存在**
    if (!std::filesystem::exists(inputFilePath) || !std::filesystem::is_regular_file(inputFilePath))
    {
        std::cerr << "Error: Input file does not exist or is not a valid file: " << inputPath << std::endl;
        return false;
    }

    std::string extension = inputFilePath.extension().string();

    // 2. **如果是 PDF，直接返回**
    if (extension == ".pdf")
    {
        std::cout << "File is already in PDF format: " << inputPath << std::endl;
        outputPath = inputPath;
        return true;
    }

    // 3. **仅支持已知的可转换格式**
    const std::vector<std::string> supportedFormats = {
        ".txt", ".docx", ".doc", ".odt", ".rtf", ".wps", ".md",
        ".pptx", ".ppt", ".odp", ".ppsx", ".pps",
        ".xlsx", ".xls", ".ods",
        ".png", ".jpg", ".jpeg", ".bmp", ".tif", ".tiff",
        ".svg",
        ".pdf" // 虽然本身就是 PDF，但可保留用于逻辑判断
    };
    if (std::find(supportedFormats.begin(), supportedFormats.end(), extension) == supportedFormats.end())
    {
        std::cerr << "Unsupported file format: " << extension << std::endl;
        return false;
    }

    // 创建临时文件夹
    std::filesystem::path tempPdfDir = createTempPdfDir(inputFilePath);

    // 执行转换命令
    if (!executeConvertCommand(inputPath, tempPdfDir))
    {
        std::cerr << "Conversion command execution failed." << std::endl;
        return false;
    }

    // 生成重命名后的文件名
    std::string newFileName = generateConvertedFileName(inputPath);
    if (newFileName.empty())
    {
        std::cerr << "Failed to generate new file name." << std::endl;
        return false;
    }

    // 构建重命名和移动目标路径
    std::filesystem::path defaultOutputPath = tempPdfDir / (inputFilePath.stem().string() + ".pdf");
    std::filesystem::path targetPath = inputFilePath.parent_path().parent_path() / "files" / newFileName;

    // 重命名并移动文件
    if (!renameAndMoveFile(defaultOutputPath, targetPath))
    {
        std::cerr << "Rename and move command execution failed." << std::endl;
        return false;
    }

    // 删除临时文件夹
    if (!deleteTempDir(tempPdfDir))
    {
        std::cerr << "Delete temp directory command execution failed." << std::endl;
        return false;
    }

    outputPath = targetPath.string();
    return true;
}

int main()
{
    std::string inputPath = "D:/C/printer/files/test.xlsx";
    std::string outputPath;

    FileConverter converter;
    if (converter.convertToPrintableFormat(inputPath, outputPath))
    {
        std::cout << "Conversion successful! Output file: " << outputPath << std::endl;
    }
    else
    {
        std::cerr << "Conversion failed!" << std::endl;
    }

    return 0;
}

// g++ FileConverter.cpp -o fc -std=c++17 -I../include
// ./fc