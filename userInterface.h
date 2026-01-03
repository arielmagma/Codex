#include <iostream>
#include <string>
#include <vector>
#include "fileLibrary.h"

class userInterface
{
    public:
        static void printMenu(int debugMode, int selectedFile, const fileLibrary& codex);
        static void printFilterMenu(const std::vector<std::string>& filters, int selectedFilter);
        
    private:
        static std::string normalMenu(const std::string& path, const std::vector<std::string>& filters);
        static std::string printFiles(const std::vector<File>& files, int selectedFile);
        static void debugMenu(const std::string& path, const std::vector<std::string>& filters);
        static void printDebugFiles(const std::vector<File>& files, int selectedFile);
};