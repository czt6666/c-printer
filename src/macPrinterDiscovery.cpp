#include <iostream>
#include <cups/cups.h>

int main()
{
    int num_printers;
    cups_dest_t *dests;
    const char *file_to_print = "/path/to/your/file.txt"; // 要打印的文件路径
    const char *printer_name = "Your_Printer_Name";       // 选择的打印机名

    std::cout << "开始找打印机" << std::endl;

    // 获取所有的打印机
    num_printers = cupsGetDests(&dests);

    if (num_printers == 0)
    {
        std::cout << "No printers found." << std::endl;
    }
    else
    {
        std::cout << "Available Printers:" << std::endl;
        for (int i = 0; i < num_printers; i++)
        {
            std::cout << "- " << dests[i].name;
            if (dests[i].is_default)
            {
                std::cout << " (Default)";
            }
            std::cout << std::endl;
        }

        // 找到打印机并打印文件
        if (printer_name != nullptr)
        {
            std::cout << "开始打印文件: " << file_to_print << " 使用打印机: " << printer_name << std::endl;
            int job_id = cupsPrintFile(printer_name, file_to_print, "Print Job", 0, nullptr);
            if (job_id > 0)
            {
                std::cout << "打印作业提交成功，作业ID: " << job_id << std::endl;
            }
            else
            {
                std::cout << "打印作业提交失败。" << std::endl;
            }
        }
        else
        {
            std::cout << "没有找到有效的打印机。" << std::endl;
        }
    }

    // 释放内存
    cupsFreeDests(num_printers, dests);
    return 0;
}
