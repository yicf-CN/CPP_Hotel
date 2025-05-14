/*
 * Utility functions and constants for the hotel management system.
 * 
 * Features:
 * - Colorized console output.
 * - Safe execution of system commands.
 * - Directory and file management.
 * - Error handling and data validation.
 * - Base64 encoding and decoding.
 * - Utility functions for string manipulation and file operations.
 * 
 * Functions:
 * - color_printf: Prints colored text to the console.
 * - safe_system: Executes a system command safely by sanitizing input.
 * - init: Initializes directories for users, rooms, and extra functions.
 * - error: Handles errors by displaying a message and restarting the program.
 * - isletter: Checks if a character is a valid letter or digit.
 * - check_str: Validates a string for illegal characters.
 * - check: Reads and decodes a file's content.
 * - getint_move: Extracts an integer from a string and moves the pointer.
 * - str2int: Converts a string to an integer.
 * - int2str: Converts an integer to a string.
 * - getFiles: Retrieves a list of files from a directory.
 * - getTime: Returns the current time.
 * 
 * Usage:
 * - Include this header file to access utility functions and constants.
 * 
 * Note:
 * - This file assumes the presence of the base64.h library for encoding and decoding.
 * - Error handling functions may restart the program to ensure data integrity.
 */
#ifndef gadgets_H
#define gadgets_H
#include <bits/stdc++.h>
#include <direct.h>
#include <dirent.h>
#include "base64.h"
using namespace std;

const int color_white = 0, color_red = 1, color_green = 2, color_blue = 3, color_yellow = 4, color_purple = 5;
void color_printf(int color, const char *s, ...)
{
    char buffer[1005];
    va_list args;
    switch (color)
    {
    case 0:
        printf("\033[37m");
        break;
    case 1:
        printf("\033[31m");
        break;
    case 2:
        printf("\033[32m");
        break;
    case 3:
        printf("\033[34m");
        break;
    case 4:
        printf("\033[33m");
        break;
    case 5:
        printf("\033[35m");
        break;
    default:
        break;
    }
    va_start(args, s);
    vsnprintf(buffer, sizeof(buffer), s, args);
    va_end(args);
    printf("%s", buffer);
    printf("\033[0m");
}

void safe_system(const char *command)
{
    string real_command = command;
    string special_chars = "&|;<>`'\"$*!#(){}[]";
    size_t pos = real_command.find_first_of(special_chars);
    if (pos != std::string::npos)
        real_command = real_command.substr(0, pos);
    system(real_command.data());
}

const string userPath = "user\\";
const string roomPath = "room\\";
const string extraPath = "extra\\";
void init()
{
    char *path = (char *)userPath.data();
    _mkdir(path);
    path = (char *)roomPath.data();
    _mkdir(path);
    path = (char *)extraPath.data();
    _mkdir(path);
}

void error(string s) // before error,save the data!
{
    cout << "\033[31m" << s << "\033[0m" << endl; // red
    safe_system("pause");
    safe_system("cls");
    execl("run.exe", "run", NULL); // restart the program
    // exit(1);
}

const string legal_letter = "0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM!:=#. \\\n@\r";
inline bool isletter(char x)
{
    return legal_letter.find(x) != (size_t)-1;
    // if(x>='0'&&x<='9')
    //     return 1;
    // if(x>='a'&&x<='z')
    //     return 1;
    // if(x>='A'&&x<='Z')
    //     return 1;
    // if(x=='!'||x=='='||x=='#'||x=='%'||x=='+'||x=='/'||x=='\n'||x=='@'||x=='\r')
    //     return 1;
    // return 0;
}

void check_str(string &s)
{
    for (char c : s)
    {
        if (!isletter(c))
        {
            cout << "Illegal letter:" << c << endl;
            error("Try again!");
        }
    }
}

string check(string file_name) // return file string(decoded)
{
    ifstream file;
    file.open(file_name);
    if (file.fail())
        error("cannot open file " + file_name);
    string encode_s, decode_s;
    int magic_number = 10321, file_number; // confirm if data be broken
    if (file.fail())
    {
        error("cannot open file " + file_name);
    }
    char ifempty = file.get();
    file >> encode_s;
    file >> file_number;
    if (ifempty != '\n')
        encode_s = ifempty + encode_s;
    else
        return "";
    file.close();
    decode_s = base64_decode(encode_s);
    for (unsigned long long i = 0; i < encode_s.length(); i++)
        if (!isletter(encode_s[i]))
            error(file_name + " may be broken(encode wrong)");
    for (unsigned long long i = 0; i < decode_s.length(); i++)
    {
        if (!isletter(decode_s[i]))
            error(file_name + " may be broken(decode wrong)");
        magic_number ^= (decode_s[i] - 1624);
    }
    if (magic_number != file_number)
        error(file_name + " may be broken(magic_number wrong)");
    return decode_s;
}

int getint_move(string &s, unsigned long long &x) // will move the pointer
{
    int re = 0;
    for (; x < s.length(); x++)
    {
        if (!(s[x] >= '0' && s[x] <= '9'))
            break;
        re *= 10;
        re += s[x] - '0';
    }
    return re;
}

int str2int(string &s)
{
    int re = 0;
    for (unsigned long long x = 0; x < s.length(); x++)
    {
        if (!(s[x] >= '0' && s[x] <= '9'))
            break;
        re *= 10;
        re += s[x] - '0';
    }
    return re;
}

string int2str(int x)
{
    string re = "";
    while (x)
    {
        re = char(x % 10 + '0') + re;
        x /= 10;
    }
    return re;
}

void getFiles(string dir_path, vector<string> &files) // save into vector<string>& files
{
    DIR *dr;
    struct dirent *en;
    dr = opendir(dir_path.data());
    if (dr)
    {
        while ((en = readdir(dr)) != NULL)
        {
            if (strcmp(en->d_name, ".") == 0 || strcmp(en->d_name, "..") == 0)
                continue;
            files.push_back(en->d_name);
        }
        closedir(dr);
    }
    return;
}

inline time_t getTime()
{
    return time(NULL);
}
#endif