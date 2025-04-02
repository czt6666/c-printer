#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#else
#include <gtk/gtk.h>
#endif

// Abstract file selector class
class FileSelector
{
public:
    virtual std::string selectFile(long long maxFileSize) = 0;
    virtual ~FileSelector() = default;
};

#ifdef _WIN32
// Windows file selector
class WindowsFileSelector : public FileSelector
{
public:
    std::string selectFile(long long maxFileSize) override
    {
        SetConsoleOutputCP(CP_UTF8);
        OPENFILENAME ofn;
        char szFile[260] = {0};

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn) == TRUE)
        {
            std::string filePath(szFile);
            struct stat fileStat;
            if (stat(filePath.c_str(), &fileStat) == 0 && fileStat.st_size <= maxFileSize)
            {
                return filePath;
            }
            std::cerr << "File size exceeds the limit or unable to retrieve file info." << std::endl;
        }
        return "";
    }
};
#else

// macOS & Linux file selector
class UnixFileSelector : public FileSelector
{
public:
    std::string selectFile(long long maxFileSize) override
    {
        gtk_init(NULL, NULL);
        GtkWidget *dialog = gtk_file_chooser_dialog_new("Select File", NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "Cancel", GTK_RESPONSE_CANCEL,
                                                        "Open", GTK_RESPONSE_ACCEPT, NULL);
        gint result = gtk_dialog_run(GTK_DIALOG(dialog));
        std::string filePath;

        if (result == GTK_RESPONSE_ACCEPT)
        {
            GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
            char *filename = gtk_file_chooser_get_filename(chooser);
            if (filename)
            {
                filePath = filename;
                g_free(filename);
            }
        }
        gtk_widget_destroy(dialog);

        struct stat fileStat;
        if (!filePath.empty() && stat(filePath.c_str(), &fileStat) == 0 && fileStat.st_size <= maxFileSize)
        {
            return filePath;
        }
        std::cerr << "File size exceeds the limit or unable to retrieve file info." << std::endl;
        return "";
    }
};
#endif

// File selector factory class
class FileSelectorFactory
{
public:
    static std::unique_ptr<FileSelector> createFileSelector()
    {
#ifdef _WIN32
        return std::make_unique<WindowsFileSelector>();
#else
        return std::make_unique<UnixFileSelector>();
#endif
    }
};

// Handles file selection logic
std::string handleFileSelection(long long maxFileSize)
{
    auto selector = FileSelectorFactory::createFileSelector();
    std::string filePath = selector->selectFile(maxFileSize);

    if (filePath.empty())
    {
        std::cout << "Unable to select a file. Please enter the file path manually: ";
        std::getline(std::cin, filePath);
        struct stat fileStat;
        if (stat(filePath.c_str(), &fileStat) != 0 || fileStat.st_size > maxFileSize)
        {
            std::cerr << "Invalid file path or file size exceeds the limit." << std::endl;
            filePath.clear();
        }
    }
    return filePath;
}

// int main()
// {
//     constexpr long long maxFileSize = 10 * 1024 * 1024; // 10MB file size limit
//     std::string filePath = handleFileSelection(maxFileSize);
//     if (!filePath.empty())
//     {
//         std::cout << "Selected file path: " << filePath << std::endl;
//     }
//     else
//     {
//         std::cout << "No valid file selected." << std::endl;
//     }
//     return 0;
// }

// g++ -std=c++17 -I. FileSelecter.cpp -lcomdlg32 -o fs
// g++ -std=c++17 -I. FileSelecter.cpp -o file_selector `pkg-config --cflags --libs gtk+-3.0`

// brew install gtk+3
// sudo apt-get install libgtk-3-dev