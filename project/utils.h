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

#endif // UTILS_H
