#include <iostream>
#include <string>
#include <memory>
#include <sys/stat.h> // For stat/_stat64

// Platform-specific includes and utility functions
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include "utils.h" // Windows-specific conversion functions reside here
#else
#include <gtk/gtk.h>
#include <cstdio> // For perror
#endif

class FileSelector
{
public:
    virtual std::string selectFile(long long maxFileSize) = 0;
    virtual ~FileSelector() = default;
};

// --- Platform-Specific Implementations ---
#ifdef _WIN32
class WindowsFileSelector : public FileSelector
{
public:
    std::string selectFile(long long maxFileSize) override
    {
        OPENFILENAMEW ofn;
        wchar_t szFile[MAX_PATH] = {0};

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_DONTADDTORECENT;

        if (GetOpenFileNameW(&ofn) == TRUE)
        {
            std::wstring wFilePath(szFile);
            struct _stat64 fileStat;

            if (_wstat64(wFilePath.c_str(), &fileStat) == 0)
            {
                if (fileStat.st_size <= maxFileSize)
                {
                    return wstring_to_utf8(wFilePath); // Convert valid path to UTF-8
                }
                else
                {
                    std::cerr << "Error: File size (" << fileStat.st_size << ") exceeds limit (" << maxFileSize << ")." << std::endl;
                }
            }
            else
            {
                std::wcerr << L"Error: Cannot get file status for: " << wFilePath << std::endl;
            }
        }
        // Return empty if dialog cancelled or error occurred
        return "";
    }
};

#else // Assuming Linux/macOS with GTK

class UnixFileSelector : public FileSelector
{
public:
    std::string selectFile(long long maxFileSize) override
    {
        if (!gtk_is_initialized())
        {
            // Note: Proper GTK app structure would init/run loop differently
            // This simple init works for a single dialog call but isn't ideal for complex apps.
            gtk_init(NULL, NULL);
        }

        GtkWidget *dialog = gtk_file_chooser_dialog_new("Select File", NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        "Cancel", GTK_RESPONSE_CANCEL,
                                                        "Open", GTK_RESPONSE_ACCEPT, NULL);
        gint result = gtk_dialog_run(GTK_DIALOG(dialog));
        std::string filePath; // Expect UTF-8

        if (result == GTK_RESPONSE_ACCEPT)
        {
            GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
            char *filename_ptr = gtk_file_chooser_get_filename(chooser);
            if (filename_ptr)
            {
                filePath = filename_ptr; // Copy UTF-8 path
                g_free(filename_ptr);
            }
        }
        gtk_widget_destroy(dialog);
        // Minimal event processing to help cleanup dialog resources
        while (gtk_events_pending())
        {
            gtk_main_iteration();
        }

        if (filePath.empty())
        {
            // Don't report error if user simply cancelled
            if (result != GTK_RESPONSE_CANCEL)
            {
                std::cerr << "Error: Could not retrieve file path from dialog." << std::endl;
            }
            return "";
        }

        struct stat fileStat;
        if (stat(filePath.c_str(), &fileStat) == 0)
        {
            if (fileStat.st_size <= maxFileSize)
            {
                return filePath; // Return UTF-8 path
            }
            else
            {
                std::cerr << "Error: File size (" << fileStat.st_size << ") exceeds limit (" << maxFileSize << ") for: " << filePath << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Cannot get file status for: " << filePath << std::endl;
            perror("stat failed");
        }

        return ""; // Return empty if checks fail
    }
};
#endif

// --- Factory ---
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

// --- Helper Function ---
// Selects a file using the appropriate platform-specific dialog.
// Returns the path as a UTF-8 encoded std::string, or empty string on failure/cancel.
std::string handleFileSelection(long long maxFileSize)
{
    auto selector = FileSelectorFactory::createFileSelector();
    // selectFile implementations now handle size checking and return UTF-8 path
    return selector->selectFile(maxFileSize);
}

// g++ -std=c++17 -I. FileSelecter.cpp -lcomdlg32 -o fs
// g++ -std=c++17 -I. FileSelecter.cpp -o file_selector `pkg-config --cflags --libs gtk+-3.0`

// brew install gtk+3
// sudo apt-get install libgtk-3-dev