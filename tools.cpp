/**
 * A utility tool for reading and processing file contents.
 *
 * This tool supports reading files in plain text or Base64 format, searching for specific strings within files,
 * and reading log files to find lines containing a target string.
 *
 * Features:
 * - Reads file content and prints it to the console.
 * - Decodes Base64 content from a file and prints the decoded data.
 * - Searches for a specific string within a file and prints matching lines.
 * - Reads log files and finds lines containing a target string.
 *
 * Usage:
 * - Compile the tool using a C++ compiler.
 * - Run the tool with appropriate command-line arguments.
 *
 * Command-line Arguments:
 * - -b: Read and decode Base64 content from the file.
 * - -r: Read file content in plain text (conflicts with -log).
 * - -log: Read log file content (conflicts with -r and -b).
 * - -f: Find specific strings within the file content (requires a target string).
 * - {file name}: The name of the file to process.
 * - {strings}: The target string to search for when using the -f option.
 *
 * Example:
 * - tools -b filename.txt: Reads and decodes Base64 content from filename.txt.
 * - tools -r filename.txt: Reads and prints plain text content from filename.txt.
 * - tools -log log.txt -f "target_string": Searches for lines containing "target_string" in log.txt.
 *
 * Note:
 * - The tool assumes the presence of the base64.h library for Base64 decoding.
 * - The tool exits with an error message if conflicting options are provided or if no file name is specified.
 */
#include <bits/stdc++.h>
#include <windows.h>
#include "base64.h"
using namespace std;

string readFile(const string &filename)
{
    ifstream in(filename, ios::binary);
    if (!in)
    {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

void findStrings(const string &content, const string &target)
{
    istringstream stream(content);
    string line;
    int lineNum = 1;
    bool found = false;

    while (getline(stream, line))
    {
        if (line.find(target) != string::npos)
        {
            cout << "Line " << lineNum << ": " << line << endl;
            found = true;
        }
        ++lineNum;
    }

    if (!found)
    {
        cout << "String not found in any line." << endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("-help for tips\n");
        return 1;
    }

    if ((argc == 2) && (strcmp(argv[1], "-help") == 0))
    {
        printf("tools <-b><-r><-log> {file name} <-f> <strings>\n");
        printf("<-b>     read with base64\n");
        printf("<-r>     read files (conflict with -log)\n");
        printf("<-log>   read logs\n");
        printf("{file name} the file's name you want to deal with\n");
        printf("<-f>     find strings (work with <strings>)\n");
        return 0;
    }

    bool useBase64 = false, readFileFlag = false, readLog = false, findStr = false;
    string filename, searchString;

    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];
        if (arg == "-b")
            useBase64 = true;
        else if (arg == "-r")
            readFileFlag = true;
        else if (arg == "-log")
            readLog = true;
        else if (arg == "-f" && i + 1 < argc)
        {
            findStr = true;
            searchString = argv[++i];
        }
        else if (arg[0] != '-')
        {
            filename = arg;
        }
    }

    if ((readFileFlag && readLog) || (useBase64 && readLog))
    {
        cout << "Error: -r(-b) and -log cannot be used together." << endl;
        return 1;
    }

    if (filename.empty())
    {
        cout << "Error: No file specified." << endl;
        return 1;
    }

    string content = readFile(filename);

    if (useBase64)
    {
        cout << "File content:\n";
        cout << content << endl;
        cout << "--------------Base 64-----------------" << endl;
        for (unsigned long long i = 0; i < content.length(); i++)
        {
            if (content[i] == '\n')
            {
                content = content.substr(0, i);
            }
        }
        content = base64_decode(content);
        cout << content << endl;
        int magic = 10321;
        for (unsigned long long i = 0; i < content.length(); i++)
        {
            magic ^= (content[i] - 1624);
        }
        cout << "Magic number should be " << magic << endl;
        return 0;
    }

    if (findStr)
    {
        findStrings(content, searchString);
    }
    else
    {
        cout << "File content:\n";
        cout << content << endl;
    }

    return 0;
}