#include <iostream>
#include <string>
#include "fileLibrary.h"

using std::cout;
using std::endl;
using std::string;

class UI
{
    public:
        static void printMenu(int debugMode, int selectedFile, const fileLibrary& codex);
        static void printFilterMenu(const vector<std::string>& filters, int selectedFilter);
        
    private:
        static void normalMenu(const string& path, const vector<std::string>& filters);
        static void printFiles(const vector<File>& files, int selectedFile);
        static void debugMenu(const string& path, const vector<std::string>& filters);
        static void printDebugFiles(const vector<File>& files, int selectedFile);
};