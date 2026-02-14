#include <iostream>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <filesystem>

#include "fileLibrary.h"

#define UP_ARROW 72
#define DOWN_ARROW 80
#define SCREEN_WIDTH 59

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

void FilterMode(fileLibrary& codex);

int main()
{
    Config settings;
    char mode = 'N';

    string str = "";
    int option = 0;
    int selectedFile = 0;
    int selectedFilter = 0;

    std::system("cls");

    fileLibrary codex(settings);
    while (true)
    {
        if (codex.getFiles().empty())
            selectedFile = 0;
        else if (selectedFile >= codex.getFiles().size())
            selectedFile = codex.getFiles().size() - 1;

        std::system("cls"); // Print ui fitting for the current mode
        if (mode == 'N')
            userInterface::printMenu(codex.getMode(), selectedFile, codex, settings.getFilesToShow());
        else if (mode == 'F')
            userInterface::printFilterMenu(codex.getFilters(), selectedFilter, settings.getFilesToShow());

        {
            int ch = getch();
            if (ch == 0 || ch == 224) // extended key prefix
                ch = getch();
            option = ch;
        }

        switch (mode)
        {
            case 'N':
                {
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
                            mode = 'F';
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
                                if (file.getFileType() == 'h')
                                {
                                    codex.updatePath(file.getFilePath());
                                }
                                if (file.getFileType() == 'd')
                                {
                                    codex.updatePath(codex.getPath() + "\\" + file.getFileName());
                                }
                                else if (file.getFileType() == 'f')
                                    codex.openFile(file);
                            }
                            break;
                        case 'B':
                        case 'b':
                            { // Check if current path is root path, else just return parent path
                                bool root = (std::filesystem::path(codex.getPath()).root_path() == std::filesystem::path(codex.getPath()));
                                root ? codex.updatePath("") : codex.updatePath(std::filesystem::path(codex.getPath()).parent_path().string());
                            }
                            break;
                        case UP_ARROW: // Up arrow
                            selectedFile--; 
                            if (selectedFile < 0) selectedFile = codex.getFiles().size() - 1;
                            break;
                        case DOWN_ARROW: // Down arrow
                            selectedFile++;
                                if (selectedFile >= codex.getFiles().size()) selectedFile = 0;
                                break;
                        case 'C':
                        case 'c':
                            std::string temp;
                            cout << "Enter 1 to set current path as base, enter 2 to update files to show" << std::endl;
                            cin >> temp;
                            if (temp == "1")
                                settings.setBasicPath(codex.getPath());
                            else if (temp == "2")
                            {
                                std::cout << "Enter how much files to show: ";
                                std::cin >> option;
                                settings.setFilesToShow(option);
                                codex.updateScreenSize();
                            }

                            break;
                        }
                    }
                    break;
                case 'F':
                    FilterMode(codex);
                    mode = 'N';
                    break;
        }
    }

    return 0;
}

void FilterMode(fileLibrary& codex)
{
    int selectedFilter = 0;
    int option = ' ';

    while (true)
    {
        system("cls");
        userInterface::printFilterMenu(codex.getFilters(), selectedFilter, 9);
        option = getch();

        switch (option)
        {
            case 'B':
            case 'b':
                return;
                break;
            case 'A':
            case 'a':
            {
                cout << "| Add New Filter                                          |" << endl;
                cout << "| Filter Types:  [N] Name   [E] Extension   [T] Type      |" << endl;
                cout << "| [S] Size                                                |" << endl;
                cout << "+---------------------------------------------------------+" << endl;

                {
                    int sub = getch();
                    if (sub == 0 || sub == 224)
                        sub = getch();
                    switch(sub)
                    {
                        case 'N':
                        case 'n':
                        {
                            string filter;
                            cout << "Enter name filter: ";
                            std::getline(cin >> std::ws, filter);
                            codex.addFilter(filter, 0);
                        }
                            break;
                        case 'E':
                        case 'e':
                        {
                            string filter;
                            cout << "Enter extension filter: ";
                            std::getline(cin >> std::ws, filter);
                            codex.addFilter(filter, 1);
                        }
                            break;
                        case 'S':
                        case 's':
                        {
                            string filter;
                            cout << "Enter size filter: ";
                            std::getline(cin >> std::ws, filter);
                            codex.addFilter(filter, 2);
                        }
                            break;
                        case 'T':
                        case 't':
                        {
                            string filter;
                            cout << "Enter type filter: ";
                            std::getline(cin >> std::ws, filter);
                            codex.addFilter(filter, 3);
                        }
                            break;
                    }
                }
            }
                break;
            case 'R':
            case 'r':
            {
                if (codex.getFilters().size() == 0)
                    continue;
                else
                    codex.removeFilter(codex.getFilters()[selectedFilter]);
                break;
            case UP_ARROW: // Up arrow
                selectedFilter--;
                if (selectedFilter < 0)
                    selectedFilter = codex.getFilters().size() - 1;
                break;
            case DOWN_ARROW: // Down arrow
                selectedFilter++;
                    if (selectedFilter >= codex.getFilters().size())
                        selectedFilter = 0;
                    break;
            }
        }
    }
}