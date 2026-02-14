#pragma once
#include <iostream>
#include <string>
#include <vector>

class fileLibrary;
class File;

class userInterface
{
    public:
        static void printMenu(int debugMode, int selectedFile, fileLibrary& codex, int WINDOW);
        static void printFilterMenu(const std::vector<std::string>& filters, int selectedFilter, int WINDOW);
        static void updateScreenSize(int x, int y);
    
    private:
        static std::string normalMenu(const std::string& path, const std::vector<std::string>& filters);
        static std::string printFiles(const std::vector<File>& files, int selectedFile, int WINDOW);
        static void debugMenu(const std::string& path, const std::vector<std::string>& filters);
        static void printDebugFiles(const std::vector<File>& files, int selectedFile);
};