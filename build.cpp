/*
 * A utility program to build a new hotel.
 * 
 * Features:
 * - Rewrites the ELF header of run.exe with the hotel name.
 * - Creates a new directory for the hotel.
 * - Copies run.exe and init.exe to the new directory.
 * - Executes init.exe to initialize the hotel.
 * - Deletes init.exe after initialization.
 * - Clears the rewritten ELF header.
 * 
 * Functions:
 * - rewriteELF(): Rewrites the ELF header of run.exe with the hotel name.
 * - clearELF(): Clears the rewritten ELF header.
 * - checkhotelname(): Checks if the hotel name already exists.
 * - newdir(): Creates a new directory for the hotel.
 * - copyELF(): Copies run.exe and init.exe to the new directory.
 * - runinitexe(): Executes init.exe to initialize the hotel.
 * - delinit(): Deletes init.exe from the new directory.
 * 
 * Usage:
 * - Run the program with the hotel name as the first argument.
 * - The hotel name should not exceed 49 characters.
 * 
 * Note:
 * - This program assumes that run.exe and init.exe are in the current directory.
 * - The hotel name should not contain special characters that could affect command execution.
 */
#include <bits/stdc++.h>
#include <windows.h>
#include "gadgets.h"
using namespace std;
char hotelname[50];
int offset = 0x1aa20;
void getoffset();
void rewriteELF()
{
    fstream elf;
    elf.open("run.exe", ios::in | ios::out | ios::binary);
    if (elf.fail())
    {
        printf("Fail to open run.exe");
        return;
    }
    elf.seekp(offset, ios::beg);
    elf.write(hotelname, sizeof(hotelname));
    color_printf(color_green, "Finish rewrite the ELF!\n");
    elf.close();
    Sleep(500);
}
void clearELF()
{
    fstream elf;
    elf.open("run.exe", ios::in | ios::out | ios::binary);
    if (elf.fail())
    {
        printf("Fail to open run.exe");
        return;
    }
    elf.seekp(offset, ios::beg);
    char nop[50];
    memset(nop, 0, sizeof(nop));
    elf.write(nop, sizeof(nop));
    color_printf(color_green, "Finish clear the ELF!\n");
    elf.close();
    Sleep(500);
}
bool checkhotelname()
{
    string name = hotelname;
    vector<string> name_list;
    getFiles("./", name_list);
    for (string i : name_list)
    {
        if (i == name)
            return 1;
    }
    return 0;
}
void newdir()
{
    _mkdir(hotelname);
    color_printf(color_green, "Finish mkdir!\n");
    Sleep(500);
}
void copyELF()
{
    string name = hotelname;
    string command = "copy run.exe .\\" + name;
    safe_system(command.data());
    command = "copy init.exe .\\" + name;
    safe_system(command.data());
    color_printf(color_green, "Finish copy ELF!\n");
    Sleep(500);
}
void runinitexe()
{
    string name = hotelname;

    string special_chars = "&|;<>`'\"$*!#(){}[]";
    size_t pos = name.find_first_of(special_chars);
    if (pos != std::string::npos)
        name = name.substr(0, pos);
    // here i must use & to init,sry
    string command = "cd " + name + "& .\\init.exe " + name;
    // string command=".\\"+name+"\\init.exe "+name;
    system(command.data());
    color_printf(color_green, "Finish run init.exe!\n");
    Sleep(500);
}
void delinit()
{
    string name = hotelname;
    string command = "del .\\" + name + "\\init.exe";
    safe_system(command.data());
    color_printf(color_green, "Finish del init.exe!\n");
    Sleep(500);
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        exit(1);
    }
    printf("building\n");
    memset(hotelname, 0, sizeof(hotelname));
    strncpy(hotelname, argv[1], 49);
    printf("Input the hotel name(max size=49): %s\n", hotelname);
    // read(0,hotelname,49);
    hotelname[49] = '\x00';
    for (int i = 0; i < 50; i++)
        if (hotelname[i] == '\n')
            hotelname[i] = '\x00'; // clear extra \n

    if (checkhotelname())
    {
        printf("This hotel already exist!");
        return 0;
    }
    printf("Now I will rewrite the ELF\n");
    rewriteELF();
    printf("Now I will make a new dir\n");
    newdir();
    printf("Now I will copy run.exe and init.exe to the dir\n");
    copyELF();
    printf("Now I will exec init.exe\n");
    runinitexe();
    printf("Now I will del init.exe in the new dir\n");
    delinit();
    clearELF();
    color_printf(color_green, "Finish building\n");
    Sleep(1500);
    return 0;
}