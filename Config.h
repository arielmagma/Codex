#ifndef CODEX_CONFIG_H
#define CODEX_CONFIG_H

#include <string>

class Config
{
    private:
        int filesToShow;
        std::string basicPath;

    public:
        Config();
        ~Config();

        int getFilesToShow() const;
        void setFilesToShow(int filesToShow);

        std::string getBasicPath() const;
        void setBasicPath(const std::string& basicPath);

    private:
        static bool startsWith(const std::string & s, const std::string & prefix);
        static std::string getStringAfter(const std::string & s, const std::string & prefix);
};


#endif