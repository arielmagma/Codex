#include "Config.h"
#include <fstream>
#include <iostream>

Config::Config()
{
    std::ifstream file("config.txt");
    std::string line;

    while (getline(file,line))
    {
        if (startsWith(line, "FILES_TO_SHOW="))
        {
            filesToShow =  std::stoi(getStringAfter(line, "FILES_TO_SHOW="));
        }
        else if (startsWith(line, "BASE_PATH="))
        {
            basePath = getStringAfter(line, "BASE_PATH=");
        }
    }

    file.close();
}

Config::~Config()
{
    std::ofstream file("config.txt");

    file << "FILES_TO_SHOW=" << filesToShow << std::endl;
    file << "BASE_PATH=" << basePath << std::endl;

    file.close();
}

bool Config::startsWith(const std::string &s, const std::string &prefix)
{
    if (s.size() < prefix.size()) return false;

    for (int i = 0; i < prefix.size(); i++)
    {
        if (prefix[i] != s[i]) return false;
    }

    return true;
}

std::string Config::getStringAfter(const std::string &s, const std::string &prefix)
{
    std::string result;

    if (prefix.size() > s.size()) return {};

    for (int i = 0; i < s.size(); i++)
    {
        if (i >= prefix.size()) result += s[i];
    }

    return result;
}

int Config::getFilesToShow() const
{
    return filesToShow;
}

void Config::setFilesToShow(int filesToShow)
{
    this->filesToShow = filesToShow;
}

std::string Config::getBasePath() const
{
    return basePath;
}

void Config::setBasePath(const std::string& basePath)
{
    this->basePath = basePath;
}

std::vector<std::string> Config::getConfigSettings()
{
    std::vector<std::string> configSettings;
    configSettings.push_back("Base Path: " + basePath);
    configSettings.push_back("Display Window: " + std::to_string(filesToShow));
    return configSettings;
}