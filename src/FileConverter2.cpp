#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib> // 用于调用外部工具
#include <fstream> // 用于文件检查
#include <functional>
#include <sstream>
#include <iomanip>

// 定义转换命令常量
#ifdef _WIN32
const std::string SYSTEM_CONVERT_COMMAND = "soffice --headless --convert-to pdf";
const std::string MOVE_COMMAND = "move";
const std::string DELETE_DIR_COMMAND = "rmdir /s /q";
#else
const std::string SYSTEM_CONVERT_COMMAND = "libreoffice --headless --convert-to pdf";
const std::string MOVE_COMMAND = "mv";
const std::string DELETE_DIR_COMMAND = "rm -r";
#endif

// 支持的文件格式列表
const std::vector<std::string> SUPPORTED_FORMATS = {
    ".txt", ".docx", ".doc", ".odt", ".rtf", ".wps", ".md",
    ".pptx", ".ppt", ".odp", ".ppsx", ".pps",
    ".xlsx", ".xls", ".ods",
    ".png", ".jpg", ".jpeg", ".bmp", ".tif", ".tiff",
    ".svg",
    ".pdf"};

// 输出错误信息
void printError(const std::string &message)
{
    std::cerr << "Error: " << message << std::endl;
}

// 执行系统命令
bool executeCommand(const std::string &command)
{
    std::cout << "Executing command: " << std::endl
              << command << std::endl
              << std::endl;
    int result = std::system(command.c_str());
    return result == 0;
}

// 计算文件的哈希值
std::string getFileHash(const std::string &inputPath)
{
    std::ifstream file(inputPath, std::ios::binary);
    if (!file)
    {
        printError("Failed to open file for hashing: " + inputPath);
        return "";
    }

    std::hash<std::string> hash_fn;
    size_t hash = 0;
    const size_t bufferSize = 4096;
    char buffer[bufferSize];

    while (file.read(buffer, bufferSize))
    {
        std::string chunk(buffer, bufferSize);
        hash ^= hash_fn(chunk) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }

    if (file.gcount() > 0)
    {
        std::string chunk(buffer, file.gcount());
        hash ^= hash_fn(chunk) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }

    std::stringstream hashStream;
    hashStream << std::hex << std::setw(16) << std::setfill('0') << hash;
    return hashStream.str();
}

// 生成转换后的文件名，添加哈希值
std::string generateConvertedFileName(const std::string &inputPath)
{
    std::filesystem::path inputFilePath(inputPath);
    std::string fileHash = getFileHash(inputPath);
    if (fileHash.empty())
    {
        printError("Failed to get file hash for: " + inputPath);
        return "";
    }
    return inputFilePath.stem().string() + "-" + fileHash + ".pdf";
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

class FileConverter
{
public:
    bool convertToPrintableFormat(const std::string &inputPath, std::string &outputPath)
    {
        std::filesystem::path inputFilePath(inputPath);

        // 检查文件是否存在
        if (!std::filesystem::exists(inputFilePath) || !std::filesystem::is_regular_file(inputFilePath))
        {
            printError("Input file does not exist or is not a valid file: " + inputPath);
            return false;
        }

        std::string extension = inputFilePath.extension().string();

        // 如果是 PDF，直接返回
        if (extension == ".pdf")
        {
            std::cout << "File is already in PDF format: " << inputPath << std::endl;
            outputPath = inputPath;
            return true;
        }

        // 仅支持已知的可转换格式
        if (std::find(SUPPORTED_FORMATS.begin(), SUPPORTED_FORMATS.end(), extension) == SUPPORTED_FORMATS.end())
        {
            printError("Unsupported file format: " + extension);
            return false;
        }

        // 创建临时文件夹
        std::filesystem::path tempPdfDir = createTempPdfDir(inputFilePath);

        // 执行转换命令
        std::string convertCommand = SYSTEM_CONVERT_COMMAND + " \"" + inputPath + "\" --outdir \"" + tempPdfDir.string() + "\"";
        if (!executeCommand(convertCommand))
        {
            printError("Conversion command execution failed.");
            return false;
        }

        // 生成重命名后的文件名
        std::string newFileName = generateConvertedFileName(inputPath);
        if (newFileName.empty())
        {
            printError("Failed to generate new file name.");
            return false;
        }

        // 构建重命名和移动目标路径
        std::filesystem::path defaultOutputPath = tempPdfDir / (inputFilePath.stem().string() + ".pdf");
        std::filesystem::path targetPath = inputFilePath.parent_path().parent_path() / "files" / newFileName;

        // 重命名并移动文件
        std::string moveCommand = MOVE_COMMAND + " \"" + defaultOutputPath.string() + "\" \"" + targetPath.string() + "\"";
        if (!executeCommand(moveCommand))
        {
            printError("Rename and move command execution failed.");
            return false;
        }

        // 删除临时文件夹
        std::string deleteCommand = DELETE_DIR_COMMAND + " \"" + tempPdfDir.string() + "\"";
        if (!executeCommand(deleteCommand))
        {
            printError("Delete temp directory command execution failed.");
            return false;
        }

        outputPath = targetPath.string();
        return true;
    }
};

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

// g++ FileConverter2.cpp -o fc2 -std=c++17 -I../include
// ./fc2
