#pragma once
#include <string>
#include <vector>
#include <cstdint>

using std::string;

class File
{
    private:
        char fileType; // 'f' for file, 'd' for directory
        string fileName;
        string filePath;
        string fileExtension;
        std::uintmax_t realFileSize;
        int fileSize;
        string fileSizeType;

    public:
        File(char fileType, const string& fileName, const string& filePath, const string& fileExtension, const std::uintmax_t& fileSize);

        const string& getFileName() const;
        const string& getFilePath() const;
        const string& getFileExtension() const;
        const int getRealFileSize() const;
        const std::uintmax_t getFileSize() const;
        const string& getFileSizeType() const;
        const char getFileType() const;

        friend std::ostream& operator<<(std::ostream& os, const File& file);
        void printFile() const;
        const string getPrintFile() const;
};