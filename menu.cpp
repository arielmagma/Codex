#include <iostream>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <filesystem>
#include <cstdlib>
#include "fileLibrary.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

void printMenu(int mode, int selectedFile, const fileLibrary& codex);
void normalMenu(const string& path, const vector<std::string>& filters);
void printFiles(const vector<File>& files, int selectedFile);
void debugMenu(const string& path, const vector<std::string>& filters);
void printDebugFiles(const vector<File>& files, int selectedFile);
void filterMenu(fileLibrary& codex);
void printFilterMenu(const vector<std::string>& filters, int selectedFilter);
void openFile(File file);


int main()
{
    string str = "";
    char option = ' ';
    int selectedFile = 0;

    std::system("cls");

    cout << "Enter root folder path: ";
    cin >> str;

    fileLibrary codex(str);
    
    while (true)
    {
        std::system("cls");
        if (selectedFile >= codex.getFiles().size())
            selectedFile = codex.getFiles().size() - 1;
            
        printMenu(codex.getMode(), selectedFile, codex);

        option = getch();

        switch (option)
        {
            case 'Q':
            case 'q':
                std::system("cls");
                return 0;
                break;
            case 'R':
            case 'r':
                codex.scanFolder();
                break;
            case 'F':
            case 'f':
                filterMenu(codex);
                break;
            case 'D':
            case 'd':
                codex.setMode(!codex.getMode());
                break;
            case 'U':
            case 'u':
                {
                    string path = codex.getPath();

                    cout << "Enter new path: ";
                    cin >> str;

                    if (str == ".." && std::filesystem::path(path).has_parent_path())
                        path = std::filesystem::path(path).parent_path().string();
                    else if (std::filesystem::exists(path + str) && std::filesystem::is_directory(path + str))
                        path += str;
                    else if (std::filesystem::exists(str) && std::filesystem::is_directory(str))
                            path = str;
                    codex.updatePath(path);
                }
                break;
            case 'S':
            case 's':
                {
                    File file = codex.getFiles()[selectedFile];
                    if (file.getFileType() == 'd')
                        codex.updatePath(codex.getPath() + "\\" + file.getFileName());
                    else
                        codex.openFile(file);
                }
                break;
            case 'B':
            case 'b':
            {
                codex.updatePath(std::filesystem::path(codex.getPath()).parent_path().string());
            }
            case 72: // Up arrow
                selectedFile--; 
                if (selectedFile < 0) selectedFile = codex.getFiles().size() - 1;
                break;
            case 80: // Down arrow
                selectedFile++;
                if (selectedFile >= codex.getFiles().size()) selectedFile = 0;
                break;
        }
    }

    return 0;
}


void printMenu(int debugMode, int selectedFile, const fileLibrary& codex)
{
    if (!debugMode)
    {
        normalMenu(codex.getPath(), codex.getFilters());
        printFiles(codex.getFiles(), selectedFile);
        cout << "| Mode: Normal ";
        cout << "| Files: " << codex.getFiles().size() << " ";
        cout << "| Filters: " << codex.getFilters().size() << " ";
        for (int i = 0; i < 59 - 15 - 10 - std::to_string(codex.getFiles().size()).length() - 1 -11 - std::to_string(codex.getFilters().size()).length() - 1; i++) cout << " ";
        cout << "|\n+---------------------------------------------------------+" << endl;
    }
    else
    {
        debugMenu(codex.getPath(), codex.getFilters());
        printDebugFiles(codex.getFiles(), selectedFile);
    }
}

void normalMenu(const string& path, const vector<std::string>& filters)
{
    int lineSize = 46; // First filter's line (after "Filters: ") is 46 characters long
    int currentSize = 0;

    // Print header
    cout << "+---------------------------------------------------------+" << endl;
    if (path.length() < 43)
    {        
        cout << "| Root folder: " << path;
        for (size_t i = 0; i < 43 - path.length(); i++) cout << " ";
    }
    else
    {
        string new_path = path;
        while (new_path.length() >= 43) // While new path string is still bigger then the space for the path print
        {
            size_t pos = new_path.find('\\'); // first find the next '\' inside the path
            if (pos == std::string::npos) // If we don't find '\' inside the path, we cut the path's string itself, not go through another folder
            {
                new_path = new_path.substr(new_path.size() - 43);
                break;
            }
            new_path = "..." + new_path.substr(pos + 1); // format the new path
        }
        cout << "| Root folder: " << new_path;
        for (size_t i = 0; i < 43 - new_path.length(); i++) cout << " ";
    }
    cout << "|" << endl;

    // Print filters
    cout << "| Filters: ";
    if (filters.size() == 0)
    {
        cout << "None" ;
        for (size_t i = 0; i < 51 - 8; i++) cout << " ";
        cout << "|" << endl;
    }
    else
    {
        currentSize = 0;
        for (const auto& filter : filters)
        {
            if (currentSize + filter.length() + 1 >= lineSize)
            {
                for (int i = 0; i < lineSize - currentSize + 1; i++) 
                    cout << " ";
                cout << "|\n";

                cout << "| ";
                currentSize = 2; // "| " counts as 2 characters
                lineSize = 57; // Size of each new line after the first one
            }

            cout << filter << " ";
            currentSize += filter.length() + 1;
        }

        // Fill remaining space for the last line
        for (int i = 0; i < lineSize - currentSize + 1; i++) cout << " ";
        cout << "|" << endl;
    }
    
    cout << "+---------------------------------------------------------+" << endl;
    cout << "|  [F] Filters  [R] Refresh [Q] Quit  [D] Debug Mode      |" << endl;
    cout << "|  [U] Update Path  [S] Select Files  [B] Go Back         |" << endl;
    cout << "+---------------------------------------------------------+" << endl;
}

void printFiles(const vector<File>& files, int selectedFile)
{
    cout << "| Files:                                                  |" << endl;
    for (int i = 0; i < files.size(); i++)
    {
        const File& file = files[i];

        if (i == selectedFile)
            cout << "|  [X] ";
        else
            cout << "|  [ ] ";
        file.printFile();
        if (file.getFileType() == 'f')
        {
            if (file.getFileName().length() + file.getFileExtension().length() + 1 + std::to_string(file.getFileSize()).length() + file.getFileSizeType().length() <= 50)
                for (int i = 0; i < 50 - file.getFileName().length() - file.getFileExtension().length() - 1 - std::to_string(file.getFileSize()).length() - file.getFileSizeType().length(); i++) cout << " ";
            cout << " |" << endl;
        }
        else if (file.getFileType() == 'd')
        {
            for (int i = 0; i < 50 - file.getFileName().length(); i++) cout << " ";
            cout << " |" << endl;
        }
    }
    cout << "+---------------------------------------------------------+" << endl;
}

void debugMenu(const string& path, const vector<std::string>& filters)
{
    cout << "DEBUG MENU\n";
    cout << "Root folder: " << path << "\n";
    cout << "Active filters:\n";
    for (const auto& filter : filters)
    {
        cout << " - " << filter << endl;
    }
    if (filters.size() == 0)
    {
        cout << " - None" << endl << endl;
    }
}

void printDebugFiles(const vector<File>& files, int selectedFile)
{
    cout << "DEBUG FILES LIST:" << endl;
    for (int i = 0; i < files.size(); i++)
    {
        const File& file = files[i];

        if (i == selectedFile)
            cout << " > ";
        cout << file << endl;
    }
}


void filterMenu(fileLibrary& codex)
{
    int selectedFilter = 0;
    char option = ' ';

    while (true)
    {
        std::system("cls");
        printFilterMenu(codex.getFilters(), selectedFilter);
        option = getch();

        switch (option)
        {
            case 'B':
            case 'b':
                return;
                break;
            case 'A':
            case 'a':
                cout << "| Add New Filter                                          |" << endl;
                cout << "| Filter Types:  [N] Name   [E] Extension   [T] Type      |" << endl;
                cout << "| [S] Size                                                |" << endl;
                cout << "+---------------------------------------------------------+" << endl;

                option = getch();
                switch(option)
                {
                    case 'N':
                    case 'n':
                        {
                            string filter;
                            cout << "Enter name filter: ";
                            cin >> filter;
                            codex.addFilter(filter, 0);
                        }
                        break;
                    case 'E':
                    case 'e':
                        {
                            string filter;
                            cout << "Enter extension filter: ";
                            cin >> filter;
                            codex.addFilter(filter, 1);
                        }
                        break;
                    case 'S':
                    case 's':
                        {
                            string filter;
                            cout << "Enter size filter: ";
                            cin >> filter;
                            codex.addFilter(filter, 2);
                        }
                        break;
                    case 'T':
                    case 't':
                        {
                            string filter;
                            cout << "Enter type filter: ";
                            cin >> filter;
                            codex.addFilter(filter, 3);
                        }
                        break;
                }
                break;
            case 'R':
            case 'r':
                if (codex.getFilters().size() == 0)
                    continue;
                else
                    codex.removeFilter(codex.getFilters()[selectedFilter]);
                break;
            case 72: // Up arrow
                selectedFilter--;
                if (selectedFilter < 0)
                    selectedFilter = codex.getFilters().size() - 1;
                break;
            case 80: // Down arrow
                selectedFilter++;
                if (selectedFilter >= codex.getFilters().size())
                    selectedFilter = 0;
                break;
        }
    }
}

void printFilterMenu(const vector<std::string>& filters, int selectedFilter)
{
    cout << "+---------------------------------------------------------+" << endl;
    cout << "|                     FILTER MENU                         |" << endl;
    cout << "+---------------------------------------------------------+" << endl;
    cout << "|    [A] Add Filter    [R] Remove Filter      [B] Back    |" << endl;
    cout << "+---------------------------------------------------------+" << endl;

    if (filters.size() != 0)
        for (int i = 0; i < filters.size(); i++)
        {
            const string& filter = filters[i];
            
            if (i == selectedFilter)
                cout << "| [X] " << filter;
            else
            {
                cout << "| [";
                if (i == 0)
                    cout << "N] ";
                else if (filter.substr(0, 1) == ".")
                    cout << "E] ";
                else if (filter.substr(filter.size() - 1, filter.size()) == "]")
                    cout << "S] ";
                else 
                    cout << "T] ";

                cout << filter;
            }

            for (int i = 0; i < 52 - filter.length(); i++) cout << " ";
                cout << "|" << endl;
        }
    else
        cout << "| No active filters                                       |" << endl;
    
    cout << "+---------------------------------------------------------+" << endl;
    cout << "| Status: Filter Management                               |" << endl;
    cout << "| Use arrows keys to navigate filters                     |" << endl;
    cout << "| Remove Filter will remove the selected filter           |" << endl;
    cout << "+---------------------------------------------------------+" << endl;
}