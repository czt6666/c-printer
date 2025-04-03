## 项目概述

该项目实现了一个跨平台的打印任务管理系统，支持从文件选择、文件格式转换到打印过程的自动化。通过封装 **`PrinterManager`、`PrintJob`、`PDFConverter` 和 `FileSelector`** 四个核心类，提供了统一的界面与功能，确保了文件打印流程的高效性和可维护性。项目采用 **工厂模式**、**智能指针** 和 **跨平台兼容技术**，可以在 **Windows**、**Linux** 和 **MacOS** 系统上运行。

## 功能特点

- **跨平台兼容性**：支持 Windows、Linux 和 MacOS 系统，自动选择相应的文件选择器。
- **文件格式转换**：支持将多种文件格式转换为 PDF，使用 `soffice` 进行格式转换。
- **静默打印**：在 Windows 系统中，通过 PowerShell + SumatraPDF 实现无界面静默打印。
- **文件唯一性管理**：通过计算文件哈希值生成唯一文件名，防止文件覆盖。
- **文件选择器**：提供图形化界面选择文件，并进行文件大小检查。

## 系统要求

### **Windows 系统**
1. **安装 PowerShell**：通常预安装，确认系统是否有 PowerShell 可执行文件路径。
2. **SumatraPDF**：需要安装 SumatraPDF，并配置其路径。可以从 [SumatraPDF 官方网站](https://www.sumatrapdfreader.org/) 下载。
3. **`soffice`（LibreOffice）**：用于文件格式转换，安装 LibreOffice 并确保 `soffice` 命令可用。可从 [LibreOffice 官网](https://www.libreoffice.org/) 下载。
4. **编译工具**：安装 **MinGW** 或 **Visual Studio**，用于编译 C++ 程序。

### **Linux 系统**
1. **GTK+ 3**：需要安装 `libgtk-3-dev`，用于图形界面文件选择器。可以通过以下命令安装：
   ```bash
   sudo apt-get install libgtk-3-dev
   ```
2. **`soffice`（LibreOffice）**：安装 LibreOffice，并确保 `soffice` 命令可用：
   ```bash
   sudo apt-get install libreoffice
   ```
3. **编译工具**：安装 `g++`，用于编译 C++ 程序：
   ```bash
   sudo apt-get install g++
   ```

### **MacOS 系统**
1. **GTK+ 3**：通过 Homebrew 安装 GTK+ 3：
   ```bash
   brew install gtk+3
   ```
2. **`soffice`（LibreOffice）**：安装 LibreOffice，并确保 `soffice` 命令可用：
   ```bash
   brew install --cask libreoffice
   ```
3. **编译工具**：安装 `g++`，用于编译 C++ 程序：
   ```bash
   xcode-select --install
   ```

## 编译与运行

### **编译步骤**

1. **克隆项目**：
   ```bash
   git clone https://github.com/your-repository/project-name.git
   cd project-name
   ```

2. **Windows 编译（使用 MinGW）**：
   - 安装 MinGW，并确保 `g++` 可用。
   - 运行以下命令编译项目：
     ```bash
     
   g++ -std=c++17 -Iinclude src/Utils.cpp src/FileSelector.cpp src/PrinterManager.cpp src/PDFConverter.cpp src/PrintJob.cpp src/main.cpp -lwinspool -lcomdlg32 -o bin/printjob

     ```

3. **Linux/MacOS 编译**：
   - 安装所需依赖（GTK+ 3 和 LibreOffice）。
   - 运行以下命令编译项目：
     ```bash
     
   g++ -std=c++17 -Iinclude src/Utils.cpp src/FileSelector.cpp src/PrinterManager.cpp src/PDFConverter.cpp src/PrintJob.cpp src/main.cpp -lcups `pkg-config --cflags --libs gtk+-3.0` -o bin/printjob

     ```

4. **确保 `soffice` 可用**：在所有系统中，确保 LibreOffice 的 `soffice` 命令可用，且已添加到系统的环境变量中。

### **运行项目**

1. **执行编译好的程序**：
   ```bash
   ./bin/printjob
   ```

2. **程序启动后**：
   - 在控制台上输入需要打印的文件路径，或通过图形化界面选择文件。
   - 选择打印机并确认打印任务，程序将自动进行 PDF 格式转换和打印。

## 常见问题

### **Q1: 程序无法找到 `soffice` 命令？**

确保 LibreOffice 安装正确，并且 `soffice` 已添加到系统的 `PATH` 环境变量中。如果没有，可以手动将其路径添加到 `PATH`。

### **Q2: Windows 下出现乱码？**

如果在 Windows 上运行时遇到字符显示乱码，请确保系统支持 **UTF-8 编码**，并且 PowerShell 设置为 UTF-8 编码输出。

### **Q3: 文件大小限制？**

项目中有文件大小检查功能，确保用户选择的文件符合打印要求。如果文件过大，可以尝试拆分文件或使用其他工具进行压缩。

## 贡献

如果您对这个项目感兴趣并希望做出贡献，请提交 **Pull Request** 或 **Issue**。我们欢迎任何建议和改进。

---

