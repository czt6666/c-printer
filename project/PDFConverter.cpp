#include "PDFConverter.h"
#include "Utils.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
const std::string SYSTEM_CONVERT_COMMAND = "soffice --headless --convert-to pdf";
const std::string MOVE_COMMAND = "move";
const std::string DELETE_COMMAND = "rmdir /s /q";

#elif defined(__linux__) || defined(__APPLE__)
const std::string SYSTEM_CONVERT_COMMAND = "soffice --headless --convert-to pdf";
const std::string MOVE_COMMAND = "mv";
const std::string DELETE_COMMAND = "rm -r";
#endif

const std::vector<std::string> SUPPORTED_FORMATS = {
    ".txt", ".docx", ".doc", ".odt", ".rtf", ".wps", ".md",
    ".pptx", ".ppt", ".odp", ".ppsx", ".pps",
    ".xlsx", ".xls", ".ods",
    ".png", ".jpg", ".jpeg", ".bmp", ".tif", ".tiff",
    ".svg", ".html", ".htm", ".xml", ".xmls", ".odg",
    ".pdf"};

// 创建临时文件夹
std::filesystem::path createTempPdfDir()
{
    std::filesystem::path tempPdfDir = std::filesystem::current_path().parent_path() / "files" / "tempPdfFiles";
    std::filesystem::create_directories(tempPdfDir);
    return tempPdfDir;
}

bool PDFConverter::convertToPDF(const std::string &inputPath, std::string &outputPath)
{
    std::filesystem::path inputFilePath(inputPath);
    if (!std::filesystem::exists(inputFilePath) || !std::filesystem::is_regular_file(inputFilePath))
    {
        printError("Input file does not exist or is not a valid file: " + inputPath);
        return false;
    }
    std::string extension = inputFilePath.extension().string();
    if (extension == ".pdf")
    {
        std::cout << "File is already in PDF format: " << inputPath << std::endl;
        outputPath = inputPath;
        return true;
    }
    if (std::find(SUPPORTED_FORMATS.begin(), SUPPORTED_FORMATS.end(), extension) == SUPPORTED_FORMATS.end())
    {
        printError("Unsupported file format: " + extension);
        return false;
    }
    std::filesystem::path tempPdfDir = createTempPdfDir();
    std::string convertCommand = SYSTEM_CONVERT_COMMAND + " \"" + inputPath + "\" --outdir \"" + tempPdfDir.string() + "\"";
    if (std::system(convertCommand.c_str()) != 0)
    {
        printError("Conversion command execution failed.");
        return false;
    }
    std::string newFileName = generateConvertedFileName(inputPath);
    if (newFileName.empty())
    {
        printError("Failed to generate new file name.");
        return false;
    }
    std::filesystem::path defaultOutputPath = tempPdfDir / (inputFilePath.stem().string() + ".pdf");
    std::filesystem::path targetPath = std::filesystem::current_path().parent_path() / "files" / newFileName;
    std::string moveCommand = MOVE_COMMAND + " \"" + defaultOutputPath.string() + "\" \"" + targetPath.string() + "\"";
    if (std::system(moveCommand.c_str()) != 0)
    {
        printError("Rename and move command execution failed.");
        return false;
    }
    std::string deleteCommand = DELETE_COMMAND + " \"" + tempPdfDir.string() + "\"";
    if (std::system(deleteCommand.c_str()) != 0)
    {
        printError("Delete temp directory command execution failed.");
        return false;
    }
    outputPath = targetPath.string();
    return true;
}