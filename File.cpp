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
    else
        std::cout << this->fileName << this->fileExtension;
}

std::string File::getPrintFile() const
{
    if (this->fileType == 'f')
        return this->fileName + this->fileExtension + " " + std::to_string(this->fileSize) + this->fileSizeType;
    else
        return this->fileName + this->fileExtension;
}

std::ostream& operator<<(std::ostream& os, const File& file)
{
    os << file.getFileName() << '\t' << file.getFileExtension() << '\t' << file.getFilePath() << '\t' << file.getFileSize();
    return os;
}