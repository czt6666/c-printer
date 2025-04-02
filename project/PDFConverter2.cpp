#include "PDFConverter.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cstdlib>
#include <system_error>
#include <stdexcept>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace
{
    std::wstring utf8_to_wstring(const std::string &str)
    {
        if (str.empty())
            return std::wstring();
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
        if (size_needed <= 0)
            throw std::runtime_error("MultiByteToWideChar failed: " + std::to_string(GetLastError()));
        std::wstring wstr(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
        return wstr;
    }

    std::string wstring_to_utf8(const std::wstring &wstr)
    {
        if (wstr.empty())
            return std::string();
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
        if (size_needed <= 0)
            throw std::runtime_error("WideCharToMultiByte failed: " + std::to_string(GetLastError()));
        std::string str(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, NULL, NULL);
        return str;
    }
}
#endif

#ifdef _WIN32
const std::wstring SOFFICE_COMMAND = L"soffice";
#else
const std::string SOFFICE_COMMAND = "soffice";
#endif

const std::vector<std::string> SUPPORTED_FORMATS = {
    ".txt", ".docx", ".doc", ".odt", ".rtf", ".wps", ".md",
    ".pptx", ".ppt", ".odp", ".ppsx", ".pps", ".xlsx", ".xls", ".ods",
    ".png", ".jpg", ".jpeg", ".bmp", ".tif", ".tiff", ".svg", ".html",
    ".htm", ".xml", ".xmls", ".odg", ".pdf"};

namespace
{
    void printError(const std::string &message) { std::cerr << "Error: " << message << std::endl; }

#ifdef _WIN32
    bool executeCommand(const std::wstring &command)
    {
        STARTUPINFOW si = {sizeof(si)};
        PROCESS_INFORMATION pi;
        std::wstring cmdLine = L"cmd.exe /C " + command;

        if (!CreateProcessW(nullptr, cmdLine.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi))
        {
            DWORD err = GetLastError();
            std::wcerr << L"CreateProcess failed: " << err << std::endl;
            return false;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return exitCode == 0;
    }
#else
    bool executeCommand(const std::string &command)
    {
        return std::system(command.c_str()) == 0;
    }
#endif

    std::filesystem::path createTempDir()
    {
        std::filesystem::path tempDir = "../files/conversion_temp";
        if (!std::filesystem::exists(tempDir))
            std::filesystem::create_directory(tempDir);
        return tempDir;
    }
}

bool PDFConverter::convertToPDF(const std::string &inputPath, std::string &outputPath)
{
    std::filesystem::path inputFilePath, tempDir, finalOutputPath;
    bool success = false;
    try
    {
#ifdef _WIN32
        inputFilePath = utf8_to_wstring(inputPath);
#else
        inputFilePath = inputPath;
#endif
        if (!std::filesystem::exists(inputFilePath) || !std::filesystem::is_regular_file(inputFilePath))
        {
            printError("Invalid input file: " + inputPath);
            return false;
        }

        std::string extension = inputFilePath.extension().generic_u8string();
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        if (extension == ".pdf")
        {
            outputPath = inputPath;
            return true;
        }
        if (std::find(SUPPORTED_FORMATS.begin(), SUPPORTED_FORMATS.end(), extension) == SUPPORTED_FORMATS.end())
        {
            printError("Unsupported format: " + extension);
            return false;
        }

        tempDir = createTempDir();
#ifdef _WIN32
        std::wstring convertCommand = SOFFICE_COMMAND + L" --headless --invisible --convert-to pdf:writer_pdf_Export --outdir \"" +
                                      tempDir.wstring() + L"\" \"" + inputFilePath.wstring() + L"\"";
#else
        std::string convertCommand = SOFFICE_COMMAND + " --headless --invisible --convert-to pdf:writer_pdf_Export --outdir \"" +
                                     tempDir.generic_u8string() + "\" \"" + inputFilePath.generic_u8string() + "\"";
#endif
        if (!executeCommand(convertCommand))
        {
            printError("Conversion failed.");
            return false;
        }

        std::filesystem::path generatedPdfPath = tempDir / (inputFilePath.stem().generic_u8string() + ".pdf");
        if (!std::filesystem::exists(generatedPdfPath))
        {
            printError("Converted file missing: " + generatedPdfPath.generic_u8string());
            return false;
        }

        finalOutputPath = "../files" / generatedPdfPath.filename();
        finalOutputPath = std::filesystem::absolute(finalOutputPath);
        std::filesystem::rename(generatedPdfPath, finalOutputPath);

#ifdef _WIN32
        outputPath = wstring_to_utf8(finalOutputPath.wstring());
#else
        outputPath = finalOutputPath.generic_u8string();
#endif
        success = true;
    }
    catch (const std::exception &e)
    {
        printError(e.what());
    }
    if (!tempDir.empty())
        std::filesystem::remove_all(tempDir);
    return success;
}

// soffice --headless --invisible --convert-to pdf:writer_pdf_Export --outdir "D:\阿巴阿巴\conversion_temp" "D:\阿巴阿巴\调剂.xlsx"
// 修改逻辑，conversion_temp这个文件夹放在当前的../files/目录下。最后的文件放到../files/xxx.pdf