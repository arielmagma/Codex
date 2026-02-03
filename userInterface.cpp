#include "userInterface.h"
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::vector;

void userInterface::printFilterMenu(const vector<std::string>& filters, int selectedFilter, int WINDOW)
{
    cout << "+---------------------------------------------------------+" << endl;
    cout << "|                     FILTER MENU                         |" << endl;
    cout << "+---------------------------------------------------------+" << endl;
    cout << "|    [A] Add Filter    [R] Remove Filter      [B] Back    |" << endl;
    cout << "+---------------------------------------------------------+" << endl;

    if (filters.size() != 0)
    {    
        int start = 0;
        int end = filters.size();
        if (filters.size() > WINDOW) // Calculate the window of files to show (defined in userInterface.h)
        {
            start = selectedFilter - WINDOW / 2;
            end = start + WINDOW;

            if (start < 0)
            {
                start = 0;
                end = WINDOW;
            }

            if (end > filters.size())
            {
                end = filters.size();
                start = filters.size() - WINDOW;
            }
        }

        for (int i = start; i < end; i++) // Iterate through the files in the calculated window
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

            for (int j = 0; j < 52 - filter.length(); j++) cout << " ";
                cout << "|" << endl;
        }
    }
    else
        cout << "| No active filters                                       |" << endl;
    
    cout << "+---------------------------------------------------------+" << endl;
    cout << "| Status: Filter Management                               |" << endl;
    cout << "| Use arrows keys to navigate filters                     |" << endl;
    cout << "| Remove Filter will remove the selected filter           |" << endl;
    cout << "+---------------------------------------------------------+" << endl;
}

void userInterface::printMenu(int debugMode, int selectedFile, const fileLibrary& codex, int WINDOW)
{
    string output = "";

    if (!debugMode)
    {
        vector<File> files = codex.getFiles();
        vector<string> filters = codex.getFilters();

        output += normalMenu(codex.getPath(), filters);
        output += printFiles(files, selectedFile, WINDOW);
        output.append("| Mode: Normal ");
        output.append("| Entries: " + std::to_string(files.size()) + " ");
        output.append("| Filters: " + std::to_string(filters.size()) + " ");
        for (int i = 0; i < 59 - 40 - std::to_string(files.size()).length() - std::to_string(filters.size()).length(); i++) output.append(" ");
        output.append("|\n+---------------------------------------------------------+\n");

        cout << output;
    }
    else
    {
        debugMenu(codex.getPath(), codex.getFilters());
        printDebugFiles(codex.getFiles(), selectedFile);
    }
}

string userInterface::normalMenu(const string& path, const vector<std::string>& filters)
{
    string output = "";
    int lineSize = 46; // First filter's line (after "Filters: ") is 46 characters long
    int currentSize = 0;

    // Print header
    output.append("+---------------------------------------------------------+\n");
    if (path.length() < 43)
    {        
        output.append("| Root folder: " + path);
        for (size_t i = 0; i < 43 - path.length(); i++) output.append(" ");
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
        output.append("| Root folder: " + new_path);
        for (size_t i = 0; i < 43 - new_path.length(); i++) output.append(" ");
    }
    output.append("|\n");

    // Print filters
    output.append("| Filters: ");
    if (filters.size() == 0)
    {
        output.append("None");
        for (size_t i = 0; i < 51 - 8; i++) output.append(" ");
        output.append("|\n");
    }
    else
    {
        currentSize = 0;
        for (const auto& filter : filters)
        {
            if (currentSize + filter.length() + 1 >= lineSize)
            {
                for (int i = 0; i < lineSize - currentSize + 1; i++) output.append(" ");
                output.append("|\n");

                output.append("| ");
                currentSize = 2; // "| " counts as 2 characters
                lineSize = 57; // Size of each new line after the first one
            }

            output.append(filter + " ");
            currentSize += filter.length() + 1;
        }

        // Fill remaining space for the last line
        for (int i = 0; i < lineSize - currentSize + 1; i++) output.append(" ");
        output.append("|\n");
    }
    
    output.append("+---------------------------------------------------------+\n");
    output.append("|  [F] Filters  [R] Refresh [Q] Quit  [D] Debug Mode      |\n");
    output.append("|  [U] Update Path  [S] Select Files  [B] Go Back         |\n");
    output.append("+---------------------------------------------------------+\n");

    return output;
}

string userInterface::printFiles(const vector<File>& files, int selectedFile, int WINDOW)
{
    string output = "";
    output.append("| Files:                                                  |\n");

    int start = 0;
    int end = files.size();

    if (files.size() > WINDOW) // Calculate the window of files to show (defined in userInterface.h)
    {
        start = selectedFile - WINDOW / 2;
        end = start + WINDOW;

        if (start < 0)
        {
            start = 0;
            end = WINDOW;
        }

        if (end > files.size())
        {
            end = files.size();
            start = files.size() - WINDOW;
        }
    }

    for (int i = start; i < end; i++) // Iterate through the files in the calculated window
    {
        const File& file = files[i];

        if (i == selectedFile)
            output.append("|  [X] ");
        else
            output.append("|  [ ] ");
        output.append(file.getPrintFile());
        if (file.getFileType() == 'f')
        {
            if (file.getFileName().length() + file.getFileExtension().length() + 1 + std::to_string(file.getFileSize()).length() + file.getFileSizeType().length() <= 50)
                for (int i = 0; i < 50 - file.getFileName().length() - file.getFileExtension().length() - 1 - std::to_string(file.getFileSize()).length() - file.getFileSizeType().length(); i++) output.append(" ");
            output.append(" |\n");
        }
        else if (file.getFileType() == 'd')
        {
            for (int i = 0; i < 50 - file.getFileName().length(); i++) output.append(" ");
            output.append(" |\n");
        }
        else if (file.getFileType() == 'h')
        {
            for (int i = 0; i < 50 - file.getFilePath().length() - file.getFileName().length() - 1; i++) output.append(" ");
            output.append(" |\n");
        }
    }
    output.append("+---------------------------------------------------------+\n");
    return output;
}

void userInterface::debugMenu(const string& path, const vector<std::string>& filters)
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

void userInterface::printDebugFiles(const vector<File>& files, int selectedFile)
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
