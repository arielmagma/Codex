#include <iostream>
#include <filesystem>
#include <algorithm>
#include "fileLibrary.h"

fileLibrary::fileLibrary(const std::string& folderPath)
{
    this->folderPath = folderPath;
    this->nameFilter = "\0";
    this->fileTypeFilter = '\0';
    scanFolder();
}

void fileLibrary::updatePath(const std::string& newPath)
{
    this->folderPath = newPath;
    scanFolder();
}

void fileLibrary::setMode(int mode)
{
    this->debugMode = mode;
}

void fileLibrary::scanFolder()
{
    this->files.clear();

    for (const auto& entry : std::filesystem::directory_iterator(this->folderPath)) 
    {
        if (!entry.is_directory()) 
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

const string& fileLibrary::getPath() const
{
    return this->folderPath;
}

int fileLibrary::getMode() const
{
    return this->debugMode;
}

const std::vector<File> fileLibrary::getFiles() const
{
    std::vector <File> filteredFiles;
    for (auto& file : this->files)
    {
        if (checkNameFilter(file.getFileName()) && checkExtensionFilter(file.getFileExtension()) && checkSizeFilter(file.getRealFileSize()) && checkTypeFilter(std::string(1, file.getFileType())))
            filteredFiles.push_back(file);
    }

    return filteredFiles;
}


const bool fileLibrary::checkNameFilter(const string& name) const
{
    if (this->nameFilter == "\0") // Check if there is name filter, if not then always return true
        return true;

    else if (name.size() == this->nameFilter.size()) // If the sizes are the same, check for equality
        if (name == this->nameFilter)
            return true;
    return false;
}

const bool fileLibrary::checkExtensionFilter(const string& type) const
{
    if (this->fileExtensionFilter.size() == 0) // If there are no type filters, always return true
        return true;

    for (int i = 0; i < this->fileExtensionFilter.size(); i++) // Check if file type matches any of the tye filters
        if (type == this->fileExtensionFilter[i])
            return true;
    return false;
}

const bool fileLibrary::checkTypeFilter(const string& type) const
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

void fileLibrary::openFile(File file)
{
    string command;

    string extension = file.getFileExtension();

    if (extension == ".exe")
    {
        command = "cmd /c start \"" + file.getFileName() + "\" \"" + file.getFilePath() + "\"";
        system(command.c_str());
    }
    else if (extension == ".cpp" || extension == ".h")
    {
        command = "code \"" + file.getFilePath() + "\"";
        system(command.c_str()); 
    }
    else
    {
        command = "explorer /select, " + file.getFilePath();
        system(command.c_str());
    }
}