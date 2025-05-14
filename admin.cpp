/*
 * A simple hotel management tool.
 * Features:
 * - Create a new hotel.
 * - List all existing hotels.
 * - Exit the program.
 * 
 * Usage:
 * - Run the program and select options from the menu.
 * - Option 1: Build a new hotel by entering a unique name.
 * - Option 2: Show all hotels in the current directory.
 * - Option 0: Exit the program.
 * 
 * Dependencies:
 * - gadgets.h: Provides utility functions like color_printf and safe_system.
 * - External executables: build.exe for creating new hotels.
 */
#include <bits/stdc++.h>
#include <windows.h>
#include "gadgets.h"
using namespace std;
vector<string> file_list;
void read_file_list()
{
    file_list.clear();
    getFiles("./", file_list);
}
int menu()
{
    printf("0.exit\n");
    printf("1.build a new hotel\n");
    printf("2.show all hotels\n");
    int reg;
    cin >> reg;
    return reg;
}
void build()
{
    read_file_list();
    string hotelname;
    cout << "Hotel name:";
    cin >> hotelname;
    for (auto i : file_list)
    {
        if (i == hotelname)
        {
            color_printf(color_red, "Illegal name!\n");
            Sleep(2000);
            return;
        }
    }
    string command = ".\\build.exe " + hotelname;
    safe_system(command.data());
}
void show()
{
    read_file_list();
    for (auto i : file_list)
    {
        if (i == "build.exe" || i == "admin.exe" || i == "run.exe" || i == "init.exe")
            continue;
        cout << i << endl;
    }
}
int main()
{
    color_printf(color_yellow, "Welcome Hotels manager!\n");
    while (1)
    {
        system("cls");
        int opt = menu();
        switch (opt)
        {
        case 0:
            color_printf(color_yellow, "Bye bye~~");
            return 0;
            break;
        case 1:
            build();
            break;
        case 2:
            show();
            break;
        default:
            break;
        }
        system("pause");
    }
    return 0;
}