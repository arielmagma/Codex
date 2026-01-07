#include "File.h"
#include <iostream>
#include <algorithm>
#include <cstdint>

File::File(char fileType, const std::string& fileName, const std::string& filePath, const std::string& fileExtension, const std::uintmax_t& fileSize)
{
    this->fileType = fileType;
    this->fileName = fileName;
    this->filePath = filePath;
    this->fileExtension = fileExtension;
    this->realFileSize = fileSize;

    this->fileSizeType = "B";
    this->fileSize = fileSize;
    if (fileSize / 1024 >= 1)
    {
        this->fileSize = fileSize / 1024;
        this->fileSizeType = "KB";
    }
    if (this->fileSize / 1024 >= 1)
    {
        this->fileSize = this->fileSize / 1024;
        this->fileSizeType = "MB";
    }
    if (this->fileSize / 1024 >= 1)
    {
        this->fileSize = this->fileSize / 1024;
        this->fileSizeType = "GB";
    }
    if (this->fileSize / 1024 >= 1)
    {
        this->fileSize = this->fileSize / 1024;
        this->fileSizeType = "TB";
    }
    if (this->fileSize / 1024 >= 1)
    {
        this->fileSize = this->fileSize / 1024;
        this->fileSizeType = "PB";
    }
}

const std::string& File::getFileName() const
{
    return this->fileName;
}

const std::string& File::getFilePath() const
{
    return this->filePath;
}

const std::uintmax_t File::getRealFileSize() const
{
    return this->realFileSize;
}

const std::uintmax_t File::getFileSize() const
{
    return this->fileSize;
}

const std::string& File::getFileExtension() const
{
    return this->fileExtension;
}

const std::string& File::getFileSizeType() const
{
    return this->fileSizeType;
}

const char File::getFileType() const
{
    return this->fileType;
}

void File::printFile() const
{
    if (this->fileType == 'f')
        std::cout << this->fileName << this->fileExtension << " " << this->fileSize << this->fileSizeType;
    else if (this->fileType == 'd')
        std::cout << this->fileName << this->fileExtension;
    else if (this->fileType == 'h')
        std::cout << this->fileName << " " << this->filePath;
}

std::string File::getPrintFile() const
{
    std::string temp = "";
    if (this->fileType == 'f')
    {
        std::string output = this->fileExtension + " " + std::to_string(this->fileSize) + this->fileSizeType;
        if (this->fileName.size() + output.size() >= 50)
        {
            temp += "...";
            size_t startIdx = this->fileName.size() - (50 - output.size() - 3);
            temp += this->fileName.substr(startIdx);
            return temp + output;
        }
        return this->fileName + output;
    }
    else if (this->fileType == 'd')
    {
        std::string output = this->fileExtension;
        if (this->fileName.size() + output.size() >= 50)
        {
            temp += "...";
            size_t startIdx = this->fileName.size() - (50 - output.size() - 3);
            temp += this->fileName.substr(startIdx);
            return temp + output;
        }
        return this->fileName + output;
    }
    else if (this->fileType == 'h')
    {
        std::string output = this->filePath;
        if (this->fileName.size() + 1 + output.size() >= 50) // + 1 from space
        {
            temp += "...";
            size_t startIdx = this->fileName.size() - (50 - output.size() - 3);
            temp += this->fileName.substr(startIdx);
            return temp + output;
        }
        return this->fileName + " " + output;
    }
    
    return "Unrecognized file type";
}

std::ostream& operator<<(std::ostream& os, const File& file)
{
    os << file.getFileName() << '\t' << file.getFileExtension() << '\t' << file.getFilePath() << '\t' << file.getFileSize();
    return os;
}