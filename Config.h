#ifndef CODEX_CONFIG_H
#define CODEX_CONFIG_H

#include <string>
#include <vector>

class Config
{
    private:
        int filesToShow;
        std::string basePath;

    public:
        Config();
        ~Config();

        int getFilesToShow() const;
        void setFilesToShow(int filesToShow);

        std::string getBasePath() const;
        void setBasePath(const std::string& basicPath);

        std::vector<std::string> getConfigSettings();

    private:
        static bool startsWith(const std::string & s, const std::string & prefix);
        static std::string getStringAfter(const std::string & s, const std::string & prefix);
};


#endif