#include "UI.h"

void UI::printFilterMenu(const vector<std::string>& filters, int selectedFilter)
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

            for (int j = 0; j < 52 - filter.length(); j++) cout << " ";
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

void UI::printMenu(int debugMode, int selectedFile, const fileLibrary& codex)
{
    string output = "";

    if (!debugMode)
    {
        output += normalMenu(codex.getPath(), codex.getFilters());
        output += printFiles(codex.getFiles(), selectedFile);
        output.append("| Mode: Normal ");
        output.append("| Files: " + std::to_string(codex.getFiles().size()) + " ");
        output.append("| Filters: " + std::to_string(codex.getFilters().size()) + " ");
        for (int i = 0; i < 59 - 15 - 10 - std::to_string(codex.getFiles().size()).length() - 1 -11 - std::to_string(codex.getFilters().size()).length() - 1; i++) output.append(" ");
        output.append("|\n+---------------------------------------------------------+\n");

        cout << output;
    }
    else
    {
        debugMenu(codex.getPath(), codex.getFilters());
        printDebugFiles(codex.getFiles(), selectedFile);
    }
}

string UI::normalMenu(const string& path, const vector<std::string>& filters)
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

string UI::printFiles(const vector<File>& files, int selectedFile)
{
    string output = "";
    output.append("| Files:                                                  |\n");
    for (int i = 0; i < files.size(); i++)
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
    }
    output.append("+---------------------------------------------------------+\n");
    return output;
}

void UI::debugMenu(const string& path, const vector<std::string>& filters)
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

void UI::printDebugFiles(const vector<File>& files, int selectedFile)
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
