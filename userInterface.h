#include <iostream>
#include <string>
#include <vector>
#include "fileLibrary.h"

class userInterface
{
    public:
        static void printMenu(int debugMode, int selectedFile, const fileLibrary& codex, int WINDOW);
        static void printFilterMenu(const std::vector<std::string>& filters, int selectedFilter, int WINDOW);
        
    private:
        static std::string normalMenu(const std::string& path, const std::vector<std::string>& filters);
        static std::string printFiles(const std::vector<File>& files, int selectedFile, int WINDOW);
        static void debugMenu(const std::string& path, const std::vector<std::string>& filters);
        static void printDebugFiles(const std::vector<File>& files, int selectedFile);
};