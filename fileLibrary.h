#include "File.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class fileLibrary
{
    private:
        int debugMode = 0;
        string folderPath;
        vector<File> files;

        string nameFilter;
        vector<string> fileExtensionFilter;
        vector<string> fileLengthFilter;
        vector<string> fileSizeFilter;
        char fileTypeFilter;
        
    public:
        fileLibrary(const string& folderPath);
        void updatePath(const string& newPath);
        void setMode(int mode);

        void scanFolder();
        
        void printFiles();

        void addFilter(const string& filter, int type);
        void removeFilter(const string& filter);

        std::vector<std::string> getFilters() const;
        const string& getPath() const;
        int getMode() const;
        const std::vector<File> getFiles() const;
        void openFile(File file);
        
    private:
        const bool checkNameFilter(const string& name) const;
        const bool checkExtensionFilter(const string& type) const;
        const bool checkSizeFilter(const std::uintmax_t& size) const;
        const bool checkTypeFilter(const string& type) const;
};

#define NAME_FILTER 0
#define EXTENSION_FILTER 1
#define SIZE_FILTER 2
#define TYPE_FILTER 3