#include <iostream>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <filesystem>

#include "UI.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

void FilterMode(fileLibrary& codex);

int main()
{
    vector<fileLibrary> tabs;
    char mode = 'N';

    string str = "";
    char option = ' ';
    int selectedFile = 0;
    int selectedFilter = 0;

    std::system("cls");

    cout << "Enter root folder path: ";
    std::getline(cin, str);

    fileLibrary codex(str);

    while (true)
    {
        if (codex.getFiles().empty())
            selectedFile = 0;
        else if (selectedFile >= codex.getFiles().size())
            selectedFile = codex.getFiles().size() - 1;

        std::system("cls"); // Print ui fitting for the current mode
        if (mode == 'N')
            UI::printMenu(codex.getMode(), selectedFile, codex);
        else if (mode == 'F')
            UI::printFilterMenu(codex.getFilters(), selectedFilter);

        option = getch();
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
                            break;
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
    char option = ' ';

    while (true)
    {
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