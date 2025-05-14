/*
 * Main entry point for the hotel management system.
 * 
 * Features:
 * - Displays a colorful hotel name.
 * - Provides options for creating a visitor account or logging in.
 * - Manages user sessions and room operations.
 * - Supports various user actions such as checking in, freeing rooms, and managing the hotel.
 * 
 * Usage:
 * - Run the program to start the hotel management system.
 * - Follow the on-screen prompts to create an account, log in, or perform other actions.
 * 
 * Note:
 * - This program assumes the presence of the base64.h, gadgets.h, and user.h libraries.
 * - The hotel name is initialized to "TEST" and can be changed in the ELF file.
 */
#include <bits/stdc++.h>
#include <windows.h>
#include "base64.h"
#include "gadgets.h"
#include "user.h"
using namespace std;
char hotel_name[50] = "TEST"; // When we need,try to change this in the elf
void colorful_print_hotel_name()
{
    std::srand(std::time(0));
    for (unsigned long long i = 0; i < strlen(hotel_name); i++)
    {
        color_printf(rand() % 5 + 1, "%c", hotel_name[i]);
    }
}
int main()
{
    if (strlen(hotel_name) == 0)
    {
        color_printf(color_red, "Please run in a hotel!\n");
        Sleep(1500);
        return 0;
    }
    safe_system("cls");
    char c_opt;
    printf("Here is  ");
    colorful_print_hotel_name();
    printf("  !!!\n");
    User user;
    string user_name, user_password;
    printf("Do you need a visitor account?(Y/N)");
    cin >> c_opt;
    if (c_opt == 'y' || c_opt == 'Y')
    {
        cout << "Name:";
        cin >> user_name;
        check_str(user_name);
        user_name = "Vis_" + user_name; // to identify visitor
        cout << "Password:";
        cin >> user_password;
        for (char c : user_password)
        {
            if (!isletter(c))
            {
                cout << "Illegal letter:" << c << endl;
                error("Try again!");
            }
        }
        user.Set(0, 0, user_name, user_password);
        user.log("Create Visitor " + user_name + " (0)");
        user.Save_User();
        color_printf(color_green, "Create visitor %s success!\n", user_name.data());
        Sleep(1500);
        safe_system("cls");
    }
    else
    {
        if (c_opt != 'N' && c_opt != 'n')
            error("Unknown choice");

        printf("Please log in first\n");
        cout << "Name:";
        cin >> user_name;
        cout << "Password:";
        cin >> user_password;
        user.Load_User(user_name, user_password);
        if (user.password != user_password) // check if the user successfully login
        {
            error("Something Wrong!"); // Normally you should exit in load_user()
        }
    }
    user.Load_Room("latest");
    cout << user;
    safe_system("pause");
    while (1)
    {
        int opt = user.Menu();
        switch (opt)
        {
        case -1:
            user.Reset_password();
            break;
        case 0:
            user.Save_Room();
            color_printf(color_yellow, "Bye bye~");
            Sleep(2000);
            execl("run.exe", "run", NULL); // restart the program
            break;
        case 1:
            Show_All_Rooms();
            break;
        case 2:
            user.Check_in_room();
            break;
        case 3:
            user.Free_room();
            break;
        case 4:
            user.Hotel_Manage_Menu();
            break;
        default:
            error("Something Wrong!"); // normally opt shouldn't escape
        }
    }
    return 0;
}