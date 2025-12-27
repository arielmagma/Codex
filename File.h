#pragma once
#include <string>
#include <vector>
#include <cstdint>


class File
{
    private:
        char fileType; // 'f' for file, 'd' for directory
        std::string fileName;
        std::string filePath;
        std::string fileExtension;
        std::uintmax_t realFileSize;
        std::uintmax_t fileSize;
        std::string fileSizeType;

    public:
        File(char fileType, const std::string& fileName, const std::string& filePath, const std::string& fileExtension, const std::uintmax_t& fileSize);

        const std::string& getFileName() const;
        const std::string& getFilePath() const;
        const std::string& getFileExtension() const;
        const std::uintmax_t getRealFileSize() const;
        const std::uintmax_t getFileSize() const;
        const std::string& getFileSizeType() const;
        const char getFileType() const;

        friend std::ostream& operator<<(std::ostream& os, const File& file);
        void printFile() const;
        std::string getPrintFile() const; // No change needed here
};