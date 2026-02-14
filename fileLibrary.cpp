#include <iostream>
#include <filesystem>
#include <algorithm>

#ifdef _WIN32 // If on windows us shell execute instead of system as shell execute is more efficent
#include <windows.h>
#include <shellapi.h>
#endif

#include "fileLibrary.h"

#include "../Optic/optic.cpp"

#define SCREEN_WIDTH 59

fileLibrary::fileLibrary(const Config& settings)
{
    this->folderPath = settings.getBasicPath();
    this->settings = settings;
    this->nameFilter = "";
    this->fileTypeFilter = '\0';
    scanFolder();
    std::cout << "\033[8;" << 11 + settings.getFilesToShow() << ";" << SCREEN_WIDTH << "t";
    updateScreenSize();
}

void fileLibrary::updatePath(const std::string& newPath)
{
    folderPath = newPath;
    scanFolder();
    filteredCacheValid = false;
    updateScreenSize();
}


void fileLibrary::setMode(int mode)
{
    this->debugMode = mode;
    this->filteredCacheValid = false;
}

void fileLibrary::scanFolder()
{
    this->files.clear();

    if (!this->folderPath.empty())
    {
        for (const auto& entry : std::filesystem::directory_iterator(this->folderPath)) 
        {
            if (!entry.is_directory()) 
            {
                try
                {
                    this->files.push_back(
                    File(
                        'f',                                       // file type
                        entry.path().filename().stem().string(),   // file name
                        entry.path().string(),                     // full path
                        entry.path().extension().string(),         // extension
                        std::filesystem::file_size(entry.path()))  // size in bytes
                    );
                }
                catch(...)
                {
                    continue;
                }
            }
            else
            {
                this->files.push_back(
                File(
                    'd',
                    entry.path().filename().stem().string(),
                    entry.path().string(),
                    "",
                    0)
                );
            }
        }
    }
    else
    {
        for (const auto& entry : getDrives())
        {
            std::string label = "No Label";
            wchar_t volumeName[MAX_PATH] = {0};
            if (GetVolumeInformationW(std::wstring(entry.begin(), entry.end()).c_str(),volumeName,MAX_PATH,nullptr,nullptr,nullptr,nullptr,0))
            {
                std::wstring wname(volumeName);
                label = std::string(wname.begin(), wname.end());
            }

            this->files.push_back(
            File(
                'h',
                label,
                entry,
                "",
                0
            )
            );
        }
    }

    // Invalidate filtered cache after changing underlying file list
    this->filteredCacheValid = false;
}

void fileLibrary::printFiles()
{
    File *file = nullptr;

    if (this->debugMode)
        for (int i = 0; i < this->files.size(); i++)
        {
            file = &this->files[i];
            if (!checkNameFilter(file->getFileName()) || !checkExtensionFilter(file->getFileExtension()) || !checkSizeFilter(file->getFileSize()) || !checkTypeFilter(std::string(1, file->getFileType())))
                continue; // Check if file passes all filters - if not skip, else print.
            else
                std::cout << *file << std::endl;
        }
    else
        for (int i = 0; i < this->files.size(); i++)
        {
            file = &this->files[i];
            if (checkNameFilter(file->getFileName()) && checkExtensionFilter(file->getFileExtension()) && checkSizeFilter(file->getFileSize()) && checkTypeFilter(std::string(1, file->getFileType())))
                file->printFile(); // Check if file passes all filters - if yes print, else skip.
        }
}

void fileLibrary::addFilter(const std::string& filter, int type)
{
    switch(type)
    {
        case NAME_FILTER:
            this->nameFilter = filter;
            break;
        case EXTENSION_FILTER:
            this->fileExtensionFilter.push_back(filter);
            break;
        case SIZE_FILTER:
            this->fileSizeFilter.push_back(filter);
            break;
        case TYPE_FILTER:
            this->fileTypeFilter = filter[0];
            break;
    }
    this->filteredCacheValid = false;
}

void fileLibrary::removeFilter(const std::string& filter)
{
    if (this->nameFilter == filter) // Check if filter fits the name filter
        this->nameFilter = "";
    
    auto typeIt = std::find(this->fileExtensionFilter.begin(), this->fileExtensionFilter.end(), filter); // Check if filter is in the type filters
    if (typeIt != this->fileExtensionFilter.end())
        this->fileExtensionFilter.erase(typeIt);

    auto sizeIt = std::find(this->fileSizeFilter.begin(), this->fileSizeFilter.end(), filter); // Check if filter is in the size filters
    if (sizeIt != this->fileSizeFilter.end())
        this->fileSizeFilter.erase(sizeIt);
    
    if (this->fileTypeFilter == filter[0]) // Check if filter fits the type filter
        this->fileTypeFilter = '\0';

    this->filteredCacheValid = false;
}

std::vector<std::string> fileLibrary::getFilters() const
{
    std::vector<std::string> allFilters;

    if (this->nameFilter != "")
        allFilters.push_back(this->nameFilter);

    for (const auto& typeFilter : this->fileExtensionFilter)
        allFilters.push_back(typeFilter);

    for (const auto& sizeFilter : this->fileSizeFilter)
        allFilters.push_back(sizeFilter);

    if (this->fileTypeFilter != '\0')
        allFilters.push_back(std::string(1, this->fileTypeFilter));

    return allFilters;
}

const std::string& fileLibrary::getPath() const
{
    return this->folderPath;
}

int fileLibrary::getMode() const
{
    return this->debugMode;
}

const std::vector<File>& fileLibrary::getFiles()
{
    if (this->filteredCacheValid)
        return this->filteredCache;

    this->filteredCache.clear();
    for (const auto& file : this->files)
    {
        if (checkNameFilter(file.getFileName()) && checkExtensionFilter(file.getFileExtension()) && checkSizeFilter(file.getRealFileSize()) && checkTypeFilter(std::string(1, file.getFileType())))
            this->filteredCache.push_back(file);
    }

    this->filteredCacheValid = true;
    return this->filteredCache;
}


const bool fileLibrary::checkNameFilter(const std::string& name) const
{
    if (this->nameFilter.empty()) // no name filter => allow
        return true;

    if (name.size() == this->nameFilter.size() && name == this->nameFilter)
        return true;

    return false;
}

const bool fileLibrary::checkExtensionFilter(const std::string& type) const
{
    if (this->fileExtensionFilter.size() == 0) // If there are no type filters, always return true
        return true;

    for (int i = 0; i < this->fileExtensionFilter.size(); i++) // Check if file type matches any of the tye filters
        if (type == this->fileExtensionFilter[i])
            return true;
    return false;
}

const bool fileLibrary::checkTypeFilter(const std::string& type) const
{
    return (this->fileTypeFilter == '\0' || type[0] == this->fileTypeFilter);
}

std::uintmax_t parseSizeToBytes(const std::string& str)
{
    size_t i = 0;
    while (i < str.size() && isdigit(str[i])) i++;

    std::uintmax_t value = std::stoull(str.substr(0, i));
    std::string unit = str.substr(i);

    if (unit == "KB") return value * 1024ULL;
    if (unit == "MB") return value * 1024ULL * 1024ULL;
    if (unit == "GB") return value * 1024ULL * 1024ULL * 1024ULL;
    if (unit == "TB") return value * 1024ULL * 1024ULL * 1024ULL * 1024ULL;

    return value; // In byte form
}

const bool fileLibrary::checkSizeFilter(const std::uintmax_t& size) const
{
    if (fileSizeFilter.empty())
        return true;

    for (const auto& filter : fileSizeFilter)
    {
        if (filter.find("Bigger[") == 0)
        {
            std::string valueStr = filter.substr(7, filter.size() - 8);
            if (size > parseSizeToBytes(valueStr))
                return true;
        }
        else if (filter.find("Equal[") == 0)
        {
            std::string valueStr = filter.substr(6, filter.size() - 7);
            if (size == parseSizeToBytes(valueStr))
                return true;
        }
        else if (filter.find("Smaller[") == 0)
        {
            std::string valueStr = filter.substr(8, filter.size() - 9);
            if (size < parseSizeToBytes(valueStr))
                return true;
        }
        else if (filter.find("Range[") == 0)
        {
            std::string rangeStr = filter.substr(6, filter.size() - 7);
            size_t dashPos = rangeStr.find('-');
            if (dashPos != std::string::npos)
            {
                std::uintmax_t lower =
                    parseSizeToBytes(rangeStr.substr(0, dashPos));
                std::uintmax_t upper =
                    parseSizeToBytes(rangeStr.substr(dashPos + 1));

                if (size >= lower && size <= upper)
                    return true;
            }
        }
    }

    return false;
}

void fileLibrary::openFile(const File& file)
{
    std::string extension = file.getFileExtension();
    std::string path = file.getFilePath();

#ifdef _WIN32
    if (extension == ".exe")
    {
        ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
        return;
    }
    else if (extension == ".cpp" || extension == ".h")
    {
        // Try to open with VS Code via its CLI; falls back to ShellExecute if not available
        std::string codeCmd = "code \"" + path + "\"";
        if (system(codeCmd.c_str()) == 0)
            return;

        ShellExecuteA(NULL, "open", "code", path.c_str(), NULL, SW_SHOWNORMAL);
        return;
    }
    else if (extension == ".txt" || extension == ".log")
    {
        ShellExecuteA(NULL, "open", "notepad.exe", path.c_str(), NULL, SW_SHOWNORMAL);
        return;
    }
    else if (extension == ".png")
    {
        std::string quotedPath = "\"" + path + "\"";
        HINSTANCE result = ShellExecuteA(NULL, "open", "..Optic\\optic.exe", quotedPath.c_str(), NULL, SW_SHOWNORMAL);
        return;
    }
    else
    {
        std::string args = "/select,\"" + path + "\"";
        ShellExecuteA(NULL, "open", "explorer.exe", args.c_str(), NULL, SW_SHOWNORMAL);
        return;
    }
#else
    // Fallback for non-Windows: use system with quoted path
    if (extension == ".cpp" || extension == ".h")
    {
        std::string command = "code \"" + path + "\"";
        system(command.c_str());
    }
    else if (extension == ".txt" || extension == ".log")
    {
        std::string command = "gedit \"" + path + "\"";
        system(command.c_str());
    }
    else
    {
        std::string command = "xdg-open \"" + path + "\"";
        system(command.c_str());
    }
#endif
}

std::vector<std::string> fileLibrary::getDrives() const
{
    std::vector<std::string> drives;

    for (char letter = 'A'; letter <= 'Z'; ++letter) 
    {
        std::string root = std::string(1, letter) + ":\\";
        if (std::filesystem::exists(root))
        {
            drives.push_back(root);
        }
    }

    return drives;
}

void fileLibrary::updateScreenSize()
{
    if (this->getFiles().size() < this->settings.getFilesToShow())
    {
        int y = 11 + int((this->getFiles().size() >= settings.getFilesToShow()) ? settings.getFilesToShow() : this->getFiles().size());
        userInterface::updateScreenSize(SCREEN_WIDTH, y);
    }
    else
        userInterface::updateScreenSize(SCREEN_WIDTH, 11 + settings.getFilesToShow());
}
