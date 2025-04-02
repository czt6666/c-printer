#ifndef UTILS_H
#define UTILS_H

#include <string>

// 打印错误信息
void printError(const std::string &message);

// 执行系统命令
bool executeCommand(const std::string &command);

// 计算文件哈希值
std::string getFileHash(const std::string &inputPath);

// 生成转换后的文件名
std::string generateConvertedFileName(const std::string &inputPath);

std::string wstring_to_utf8(const std::wstring &wstr);

std::wstring utf8_to_wstring(const std::string &str);

#endif // UTILS_H
