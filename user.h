/*
 * User management class for the hotel system.
 * 
 * Features:
 * - Manages user information including ID, permission level, name, and password.
 * - Provides methods for loading and saving user data.
 * - Supports user actions such as checking in, freeing rooms, and managing the hotel.
 * - Implements a menu-driven interface for user interactions.
 * 
 * Methods:
 * - Load_User: Loads user data from a file.
 * - Save_User: Saves user data to a file.
 * - Load_Room: Loads room data from a file.
 * - Save_Room: Saves room data to a file.
 * - Reset_password: Resets the user's password.
 * - Check_in_room: Checks a customer into a room.
 * - Free_room: Frees a room.
 * - Create_User: Creates a new user.
 * - Del_User: Deletes a user.
 * - Show_all_customers: Displays all customers in the rooms.
 * - Backups_Data: Backs up room data.
 * - Load_Backups: Loads room data from a backup.
 * - ExtraFunction: Executes an extra function.
 * - Menu: Displays the main menu and handles user input.
 * - Hotel_Manage_Menu: Displays the hotel management menu and handles user input.
 * 
 * Usage:
 * - Instantiate the User class and use its methods to manage users and rooms.
 * 
 * Note:
 * - This class assumes the presence of the room.h and gadgets.h libraries.
 * - User actions are logged to a file named "log.txt".
 */
#ifndef USER_H
#define USER_H
#include "room.h"
const int Employee_permission = 2;
const int Manager_permission = 3;
class User
{
private:
    int id;
    int permission; // 0:visitor 2-Employee 3-manager
    string name;

public:
    string password;
    User(int a = 0, int b = 0, string s = "", string p = "") // id permission name pass
    {
        id = a;
        permission = b;
        name = s;
        password = p;
    }
    void Set(int a = 0, int b = 0, string s = "", string p = "")
    {
        id = a;
        permission = b;
        name = s;
        password = p;
    }
    void Load_User(string file_name, string inpass);
    void Save_User();                           //{password}{id}#{permission}{name} \n {magic_number}
    void Load_Room(string filename = "latest"); //{inuse}{type}#{number}#{begin_time}#{customer.cnt}:{name[1]}:{name[cnt]}!{nextromm}
    void Save_Room(string filename = "latest");
    void Reset_password();
    void Check_in_room();
    void Create_Room();
    void Del_Room();
    void Free_room();
    void Create_User();
    void Del_User();
    void Show_all_customers();
    void Backups_Data();
    void Load_Backups();
    void ExtraFunction();
    int Menu();
    void Hotel_Manage_Menu();
    void log(string s)
    {
        time_t now = getTime();
        ofstream logfile;
        logfile.open("log.txt", ios::app | ios::out);
        logfile << ctime(&now) << "User: " << name << " ( " << id << " )" << endl;
        logfile << s << endl;
        logfile << "---------------------------------------" << endl;
    }
    friend ostream &operator<<(ostream &, const User &);
};
ostream &operator<<(ostream &strm, const User &obj)
{
    strm << "Welcome!\n";
    if (obj.permission == 0)
        strm << "Vistor: ";
    if (obj.permission == Employee_permission)
        strm << "\033[32mEmployee\033[0m: "; // green
    // strm<<"Employee: ";
    if (obj.permission == Manager_permission)
        strm << "\033[31mManager\033[0m: "; // red
    // strm<<"Manager: ";
    strm << obj.name << "  (" << obj.id << ")\n";
    return strm;
}

void User::Load_User(string file_name, string inpass)
{
    file_name = userPath + file_name;
    string s = check(file_name);
    unsigned long long i;
    for (i = 0; i < inpass.length(); i++)
    {
        if (s[i] != inpass[i])
        {
            printf("password wrong!\n");
            Sleep(2000);
            error("Try again");
            return;
        }
    }
    password = inpass;
    for (; s[i] != '#'; i++)
    {
        id *= 10;
        id += s[i] - '0';
    }
    i++;
    permission = s[i] - '0';
    name = s.substr(i + 1, s.length() - i);
    string logdata = "Log in with " + inpass;
    log(logdata);
    // Save_User();// mabey I can add something here to backups
}

void User::Save_User() //{password}{id}#{permission}{name} \n {magic_number}
{
    string output = "";
    int magic_number = 12345;
    output += int2str(id);
    output = password + output + '#';
    output += char(permission + '0');
    output += name;
    for (unsigned long long i = 0; i < output.length(); i++)
        magic_number ^= (output[i] - 1111);
    output = base64_encode(output);
    output += '\n';
    ofstream file;
    file.open(userPath + name);
    if (file.fail())
        error("cannot open file " + userPath + name);
    file << output << magic_number;
    file.close();
    log("Save user");
}

void User::Load_Room(string filename) //{inuse}{type}#{number}#{begin_time}#{customer.cnt}:{name[1]}:{name[cnt]}!{nextromm}
{
    room_top = 0;
    memset(room_bin, 0, sizeof(room_bin));
    string str = check(roomPath + filename);
    unsigned long long pointer = 0;
    while (pointer < str.length())
    {
        room_top++;
        room[room_top].clear();
        room[room_top].Load(str, pointer);
        if (room[room_top].inuse == 0)
        {
            int p = SearchBin(room[room_top].type);
            if (p == 0)
                room_bin[room[room_top].type] = room_top;
            else
            {
                room[p].next_room = room_top;
                room[room_top].last_room = p;
            }
        }
    }
    // log("Load rooms");
}

void User::Save_Room(string filename)
{
    string output = "";
    int magic_number = 12345;
    for (int i = 1; i <= room_top; i++)
    {
        if (room[i].type == -1)
            continue;
        output += room[i].backStr();
    }
    for (unsigned long long i = 0; i < output.length(); i++)
        magic_number ^= (output[i] - 1111);
    output = base64_encode(output);
    output += '\n';
    ofstream file;
    file.open(roomPath + filename);
    if (file.fail())
        error("cannot open file " + roomPath + filename);
    file << output << magic_number;
    file.close();
    log("Save rooms");
}

void User::Reset_password()
{
    string tmp_password;
    printf("Please enter your current password.\n");
    cin >> tmp_password;
    if (tmp_password != password)
    {
        color_printf(color_red, "Wrong password!");
        Sleep(2000);
        return;
    }
    printf("Please enter your new password.\n");
    cin >> tmp_password;
    for (char c : tmp_password)
    {
        if (!isletter(c))
        {
            cout << "Illegal letter:" << c << endl;
            return;
        }
    }
    string logdata = "Set password from {" + password + "} to{" + tmp_password + "}";
    password = tmp_password;
    log(logdata);
    Save_User();
}

void User::Check_in_room()
{
    safe_system("cls");
    char opt;
    cout << "Do you need quick_check_in?(Y/N)";
    cin >> opt;
    if (opt == 'y' || opt == 'Y')
    {
        int wanted_type;
        cout << "Which type do you need:";
        cin >> wanted_type;
        int wanted_room = SearchBin(wanted_type);
        if (wanted_room == 0)
        {
            cout << "Sorry,there's no more this type of room\n";
            Sleep(1500);
            return;
        }
        printf("Find Room %d is empty,check in now?(Y/N)", room[wanted_room].number);
        cin >> opt;
        if (opt != 'Y' && opt != 'y')
            return;
        room[wanted_room].Use_room();

        string logdata;
        logdata = "Check in Room " + int2str(wanted_room) + '\n' + room[wanted_room].customer.list();
        log(logdata);

        color_printf(color_green, "Check in room %d success!\n", room[wanted_room].number);
        Save_Room("latest");
        Sleep(1500);
        return;
    }
    if (opt != 'N' && opt != 'n')
    {
        Sleep(500);
        return;
    }
    cout << "which room do you want to check in(enter -1 to exit)\n";
    while (1)
    {
        int wanted_number;
        cout << "Room number:";
        cin >> wanted_number;
        if (wanted_number == -1)
            break;
        int wanted_room = Search_Number(wanted_number);
        if (wanted_room == -1)
        {
            cout << "Sorry,There's no room " << wanted_number << "\n";
            continue;
        }
        if (room[wanted_room].inuse == 1)
        {
            cout << "Sorry,this room is in use\n";
            continue;
        }
        printf("Find Room %d is empty,check in now?(Y/N)", room[wanted_room].number);
        cin >> opt;
        if (opt != 'Y' && opt != 'y')
            continue;
        room[wanted_room].Use_room();
        color_printf(color_green, "Check in room %d success!\n", wanted_number);

        string logdata;
        logdata = "Check in Room " + int2str(wanted_room) + '\n' + room[wanted_room].customer.list();
        log(logdata);

        Save_Room("latest");
        Sleep(1500);
    }
    return;
}

void User::Create_Room()
{
    room_top++;
    room[room_top].inuse = 1;
    int x = -1;
    while (x < 0 || x > MAX_ROOMTYPE)
    {
        cout << "type:";
        cin >> x;
        if (x < 0 || x > MAX_ROOMTYPE)
        {
            printf("Invalid type!\nTry again\n");
        }
    }
    room[room_top].type = x;
    x = -1;
    while (x < 0 || x > MAX_ROOM)
    {
        cout << "number:";
        cin >> x;
        if (x < 0 || x > MAX_ROOM)
        {
            printf("Invalid number!\nTry again\n");
        }
        if (Search_Number(x) != -1)
        {
            printf("Number is existed!\nTry again\n");
        }
    }
    room[room_top].number = x;
    Room_Free(room_top);

    string logdata;
    logdata = "Create Room " + int2str(room[room_top].number) + "\n";
    logdata += "Type: " + int2str(room[room_top].type);
    log(logdata);

    sort(room + 1, room + room_top + 1);
    Save_Room();
    color_printf(color_green, "Create new room success!\n");
    Sleep(1500);
}

void User::Del_Room()
{
    int num;
    cout << "Room number:";
    cin >> num;
    int x = Search_Number(num);
    if (x == -1) // not find the room
    {
        cout << "Wrong! The room isn't exist\n";
        Sleep(1500);
        return;
    }
    if (room[x].inuse == 1) // remind the user
    {
        color_printf(color_red, "Warning! This room is in use!\nContinue?(Y/N)");
        char x;
        cin >> x;
        if (x != 'Y' && x != 'y')
            return;
    }
    room[x].type = -1;

    string logdata;
    logdata = "Del Room " + int2str(room[x].number) + "\n";
    log(logdata);

    Save_Room();
    color_printf(color_green, "Del room %d success!\n", num);
    Load_Room();
    Sleep(1500);
}

void User::Free_room()
{
    int room_number;
    cout << "Room number:";
    cin >> room_number;
    int room_id = Search_Number(room_number);
    if (room_id == 0)
    {
        color_printf(color_red, "There's no room %d\n", room_number);
        Sleep(1500);
        return;
    }
    if (room[room_id].inuse == 0)
    {
        color_printf(color_red, "Room %d already free!\n", room_number);
        Sleep(1500);
        return;
    }
    cout << "Begin time:" << ctime(&room[room_id].begin_time);
    time_t ttime = getTime();
    cout << "End time:" << ctime(&ttime);
    string logdata;
    logdata = "Free Room " + int2str(room_id);
    log(logdata);
    Room_Free(room_id);
    Save_Room("latest");
    color_printf(color_green, "Free room %d success!\n", room_number);
    Sleep(3000);
}

void User::Create_User()
{
    safe_system("cls");
    if (permission != Manager_permission)
    {
        error("Permission denied!(No enough permission)"); // abnormally exec this
    }
    vector<string> user_list; // read users
    getFiles(userPath, user_list);
    printf("What kind of user you want to create?\n");
    color_printf(color_green, "1.Employee\n");
    color_printf(color_red, "2.Manager\n");
    color_printf(color_blue, ">>");
    char c;
    cin >> c;
    if (c == '1')
    {
        User user;
        string user_name, user_password;
        int user_id;
        cout << "Id:";
        cin >> user_id;
        cout << "Name:";
        cin >> user_name;
        cout << "password:";
        cin >> user_password;
        check_str(user_name);
        check_str(user_password);
        for (auto s : user_list)
        {
            if (s == user_name)
            {
                color_printf(color_red, "This user is already exist!");
                Sleep(2000);
                return;
            }
        }
        user.Set(user_id, 2, user_name, user_password);
        string logdata;
        logdata = "Create Employee " + user_name + " ( " + int2str(user_id) + " )";
        log(logdata);
        user.Save_User();
        cout << user;
        color_printf(color_green, "Create Employee %s success!\n", user_name.data());
        Sleep(1500);
        safe_system("cls");
        return;
    }
    if (c == '2')
    {
        User user;
        string user_name, user_password;
        int user_id;
        cout << "Id:";
        cin >> user_id;
        cout << "Name:";
        cin >> user_name;
        cout << "password:";
        cin >> user_password;
        for (auto s : user_list)
        {
            if (s == user_name)
            {
                color_printf(color_red, "This user is already exist!");
                Sleep(2000);
                return;
            }
        }
        user.Set(user_id, 6, user_name, user_password);
        string logdata;
        logdata = "Create Manager " + user_name + " ( " + int2str(user_id) + " )";
        log(logdata);
        user.Save_User();
        cout << user;
        color_printf(color_green, "Create Manager %s success!\n", user_name.data());
        Sleep(1500);
        safe_system("cls");
        return;
    }
    printf("Invalid option!\n");
    return;
}

void User::Del_User()
{
    safe_system("cls");
    if (permission != Manager_permission)
    {
        error("Permission denied!(No enough permission)"); // abnormally exec this
    }
    string logdata;
    printf("Here is all users:\n");
    vector<string> user_list;
    getFiles(userPath, user_list);
    int reg = 0;
    for (auto s : user_list)
        cout << ++reg << " : " << s << endl;
    char opt;
    printf("Do you need clean all visitors?(Y/N)");
    cin >> opt;
    if (opt == 'Y' || opt == 'y')
    {
        logdata = "Ready to clean visitors";
        log(logdata);
        for (auto s : user_list)
        {
            if (s[0] == 'V' && s[1] == 'i' && s[2] == 's' && s[3] == '_')
            {
                // string sys = "cd " + userPath + " && del " + s;
                string sys = "del .\\" + userPath + "" + s;
                logdata = "Del user " + s;
                log(logdata);
                safe_system(sys.data());
                color_printf(color_green, "Del visitor %s success!\n", s.data());
            }
        }
    }
    if (opt != 'N' && opt != 'n')
    {
        color_printf(color_red, "Unknown choice\n");
        return;
    }
    int idx;
    string del_name;
    printf("Which user you want to del?(int put index)");
    cin >> del_name;
    idx = str2int(del_name);
    if (idx < 0 || idx > reg)
    {
        color_printf(color_red, "No such user!\n");
        return;
    }
    del_name = user_list[idx - 1];
    if (del_name == name)
    {
        color_printf(color_red, "You can't del yourself!\n");
        Sleep(1500);
        return;
    }
    // string sys = "cd " + userPath + " && del " + del_name;
    string sys = "del .\\" + userPath + "" + del_name;
    cout << sys << endl;
    logdata = "Del user " + del_name;
    log(logdata);
    safe_system(sys.data());
    color_printf(color_green, "Del user %s success!\n", del_name.data());
    Sleep(1500);
}

void User::Show_all_customers()
{
    string inpass;
    printf("Password:");
    cin >> inpass;
    if (inpass != password)
        error("Password wrong!");
    for (int i = 1; i <= room_top; i++)
    {
        cout << "Room " << room[i].number << ":";
        if (room[i].inuse == 0)
        {
            cout << " empty" << endl;
            continue;
        }
        cout << room[i].customer << endl;
    }
    safe_system("pause");
}

void User::Backups_Data()
{
    int nowtime = getTime();
    string filename = int2str(nowtime);
    Save_Room(filename);
    log("Save Room to " + filename);
    color_printf(color_green, "Successfully backups rooms\n");
    Sleep(2000);
}

void User::Load_Backups()
{
    vector<string> data_list;
    getFiles(roomPath, data_list);
    color_printf(color_yellow, "Here is all the backups\n");
    int reg = 0, latest_idx;
    for (auto i : data_list)
    {
        cout << ++reg << ":";
        if (i == "latest")
        {
            cout << i << endl;
            latest_idx = reg;
        }
        else
        {
            time_t now = str2int(i);
            cout << ctime(&now);
        }
    }
    color_printf(color_yellow, "Which one to load?");
    int opt;
    cin >> opt;
    if (!(0 < opt && opt <= reg))
    {
        color_printf(color_red, "Illegal choice!\n");
        Sleep(2000);
        return;
    }
    if (opt == latest_idx)
    {
        color_printf(color_green, "Successfully load latest");
        Sleep(1500);
        return;
    }
    time_t now = str2int(data_list[opt - 1]);
    Load_Room(data_list[opt - 1]);
    Save_Room();
    color_printf(color_green, "Successfully load backups when %s", ctime(&now));
    Sleep(2000);
    log("Load backups " + now);
    return;
}

void User::ExtraFunction()
{
    system("cls");
    vector<string> tmp_list, func_list;
    getFiles(extraPath, tmp_list);
    for (auto i : tmp_list)
    {
        cout << i << " " << i.substr(i.length() - 4, 4) << endl;
        if (i.substr(i.length() - 4, 4) != ".exe")
            continue;
        func_list.push_back(i.substr(0, i.length() - 4));
    }
    if (func_list.size() == 0)
    {
        color_printf(color_red, "You have no extra functions!\n");
        Sleep(2000);
        return;
    }
    color_printf(color_yellow, "Here is all your extra functions:\n");
    int index = 0;
    for (auto i : func_list)
    {
        cout << ++index << " : " << i << endl;
    }
    int choose;
    cout << "Which one do you want to use?\n";
    color_printf(color_blue, ">>");
    cin >> choose;

    string func_name = func_list[choose - 1];
    string command;
    command="cd "+extraPath+" && .\\"+func_name;//release .txt and .dat
    system(command.data());

    func_name = extraPath + func_name;
    int arg_number;
    fstream func_arg;
    func_arg.open(func_name + ".dat", ios::in);
    func_arg >> arg_number;
    if (arg_number >= 5)
        error("This function is not illegal!\n");

    string use_args;
    if (choose <= 0 || choose > index)
    {
        color_printf(color_red, "Illegal choice!\n");
        Sleep(2000);
        return;
    }

    command = "type " + func_name + ".txt";
    color_printf(color_yellow, "Here is tips for this function:\n");
    safe_system(command.data());
    color_printf(color_yellow, "\nNow I need %d arguments\n", arg_number);
    command = ".\\" + func_name + ".exe";
    for (int i = 1; i <= arg_number; i++)
    {
        cin >> use_args;
        command += " " + use_args;
    }
    safe_system(command.data());

    string logdata = "exec " + func_name;
    log(logdata);
    color_printf(color_green, "Finish exec %s", func_name.data());
    Load_Room();
    Sleep(2000);
}

int User::Menu()
{
    int choice;
    while (1)
    {
        safe_system("cls");
        printf("0.exit\n");
        printf("1.show all rooms\n");
        if (permission == Employee_permission || permission == Manager_permission)
            printf("2.check in\n3.free room\n");
        if (permission == Manager_permission)
            printf("4.Hotel Manage\n");
        printf("-1.Reset your password\n");
        // printf(">>");
        color_printf(color_blue, ">>");
        scanf("%d", &choice);
        if (-1 <= choice && choice <= permission + 1)
            return choice;
        else
        {
            printf("request illegal\n");
            Sleep(2000);
            safe_system("cls");
        }
    }
}

void User::Hotel_Manage_Menu()
{
    safe_system("cls");
    int choice;
    color_printf(color_yellow, "Hoter Manage Menu:\n");
    printf("1.Create Room\n");
    printf("2.Del_Room\n");
    printf("3.Create User\n");
    printf("4.Delete User\n");
    printf("5.Show all customers\n");
    printf("6.Backups data\n");
    printf("7.Load backups\n");
    printf("8.Extra Functions\n");
    color_printf(color_blue, ">>");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        Create_Room();
        break;
    case 2:
        Del_Room();
        break;
    case 3:
        Create_User();
        break;
    case 4:
        Del_User();
        break;
    case 5:
        Show_all_customers();
        break;
    case 6:
        Backups_Data();
        break;
    case 7:
        Load_Backups();
        break;
    case 8:
        ExtraFunction();
        break;
    default:
        color_printf(color_red, "Illegal choice!");
        Sleep(2000);
        return;
    }
}
#endif
