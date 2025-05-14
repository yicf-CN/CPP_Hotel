//import.cpp which is an extra function!
/*
 * Extra function to import room data from a file.
 * 
 * Features:
 * - Reads room data from a specified file.
 * - Initializes room information (type and number).
 * - Saves the imported room data using the User class.
 * 
 * Usage:
 * - Run the program with the file name containing room data as the first argument.
 * 
 * Note:
 * - The file should contain room_top followed by room type and number pairs.
 * - This function assumes the presence of the gadgets.h, room.h, and user.h libraries.
 */
#include <bits/stdc++.h>
#include <windows.h>
#include "gadgets.h"
#include "room.h"
#include "user.h"
using namespace std;
void importinit()
{
    fstream file;
    file.open("import.dat",ios::out);
    file<<1;
    file.close();

    file.open("import.txt",ios::out);
    file<<  "import can help you import room data from other files\n"
        <<  "please make sure your file is like this\n\n"
        <<   "3\n"
        <<   "1 1\n"
        <<  "1 2\n"
        <<  "1 3\n\n"
        <<  "3 for the room numbers\n"
        <<  "and 1 1 for room type and room number\n"
        <<  "also the 1 2 , 1 3\n"
        <<  "you need to input your file name and location\n";
    file.close();
    return;
}
int main(int argc, char *argv[]) // this is just a test code
{
    if (argc != 2)
    {
        importinit();
        exit(1);
    }
    User import(0, Manager_permission, "import", "");
    string file_name = argv[1];
    fstream file;
    file.open(file_name, ios::in);
    if (file.fail())
    {
        color_printf(color_red, "Fail to open %s", file_name);
        return 1;
    }
    file >> room_top;
    for (int i = 1; i <= room_top; i++)
    {
        file >> room[i].type >> room[i].number;
    }
    import.Save_Room();
    file.close();
    return 0;
}