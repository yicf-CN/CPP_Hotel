/*
 * Initialization utility for setting up a new user and room configuration.
 * 
 * Features:
 * - Initializes the necessary directories using the init function.
 * - Creates a new user with Manager permissions.
 * - Saves the user and room configurations.
 * - Deletes the log.txt file.
 * 
 * Usage:
 * - Run the program with a single argument (desired username).
 * 
 * Note:
 * - This utility assumes the presence of the base64.h, gadgets.h, and user.h libraries.
 * - The username provided as an argument will be used for both the username and password.
 */
#include <bits/stdc++.h>
#include <windows.h>
#include "base64.h"
#include "gadgets.h"
#include "user.h"
using namespace std;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        exit(1);
    }
    printf("initing\n");
    init();
    User test(0, Manager_permission, argv[1], argv[1]);
    test.Save_User();
    test.Save_Room();
    system("del log.txt");
    return 0;
}