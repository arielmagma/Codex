#pragma once
#include "File.h"
#include "Config.h"
#include "userInterface.h"
#include <string>
#include <vector>

class fileLibrary
{
    private:
        int debugMode = 0;
        std::string folderPath;
        std::vector<File> files;

        std::string nameFilter;
        std::vector<std::string> fileExtensionFilter;
        std::vector<std::string> fileLengthFilter;
        std::vector<std::string> fileSizeFilter;
        char fileTypeFilter;

        std::vector<File> filteredCache;
        bool filteredCacheValid = false;

        Config* settings;
        
    public:
        fileLibrary(Config* settings);
        void updatePath(const std::string& newPath);
        void setMode(int mode);

        void scanFolder();
        
        void printFiles();

        void addFilter(const std::string& filter, int type);
        void removeFilter(const std::string& filter);

        std::vector<std::string> getFilters() const;
        const std::string& getPath() const;
        int getMode() const;
        const std::vector<File>& getFiles();
        void openFile(const File& file);

        void updateScreenSize();

        std::vector<std::string> getConfigs() const;
        void setConfig(const std::vector<std::string>& configs);

        Config getConfig() const;

    private:
        const bool checkNameFilter(const std::string& name) const;
        const bool checkExtensionFilter(const std::string& type) const;
        const bool checkSizeFilter(const std::uintmax_t& size) const;
        const bool checkTypeFilter(const std::string& type) const;
        std::vector<std::string> getDrives() const;

        bool handleConfigUpdates(const std::string& field, const std::string& value);
};

#define NAME_FILTER 0
#define EXTENSION_FILTER 1
#define SIZE_FILTER 2
#define TYPE_FILTER 3