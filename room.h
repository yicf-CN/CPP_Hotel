/*
 * Room management class and utility functions.
 *
 * Features:
 * - Manages room information including usage status, type, number, and customer details.
 * - Supports loading and saving room data in a specific format.
 * - Provides methods for using and freeing rooms.
 * - Maintains a linked list structure for rooms of the same type.
 *
 * Class Methods:
 * - Use_room(): Marks a room as in use and initializes customer details.
 * - Load(string &str, unsigned long long &pointer): Loads room data from a string.
 * - backStr(): Returns a string representation of the room data for saving.
 * - clear(): Clears room data.
 *
 * Utility Functions:
 * - SearchBin(int t): Finds the last room of a given type in the linked list.
 * - Room_Free(int x): Marks a room as free and updates the linked list.
 * - Search_Number(int x): Finds a room by its number.
 * - Show_All_Rooms(): Displays information about all rooms.
 *
 * Usage:
 * - Instantiate the Room class and use its methods to manage room data.
 * - Use utility functions for searching and displaying room information.
 *
 * Note:
 * - The class assumes the presence of the gadgets.h and customer.h libraries.
 * - The Load and backStr methods use a specific format for storing and retrieving data.
 */
#ifndef ROOM_H
#define ROOM_H
#include <bits/stdc++.h>
#include "gadgets.h"
#include "customer.h"
using namespace std;
const int MAX_ROOM = 10005, MAX_ROOMTYPE = 105;
int room_bin[MAX_ROOMTYPE];
int room_top;
class Room
{
public:
    bool inuse;
    int type;
    int number;
    int next_room; // if inuse ,next_room=0;
    int last_room;
    time_t begin_time;
    Customer customer;
    Room(bool use = 0, int t = 0, int n = 0)
    {
        inuse = use;
        type = t;
        number = n;
        next_room = use == 1;
        begin_time = 0;
    }
    // inline bool getinuse(){return inuse;}
    // inline int gettype(){return type;}
    // inline int getnumber(){return number;}
    void Load(string &str, unsigned long long &pointer); //{inuse}{type}#{number}#{begin_time}#{customer}!{nextroom}
    string backStr();
    void Use_room();
    bool operator<(Room obj) const
    {
        if (type == obj.type)
            return number < obj.number;
        return type < obj.type;
    }
    void clear()
    {
        inuse = 0;
        type = 0;
        number = 0;
        next_room = 0;
        last_room = 0;
        begin_time = 0;
        customer.clear();
    }
    friend ostream &operator<<(ostream &, const Room &);
};
Room room[MAX_ROOM];

void Room::Use_room()
{
    int n;
    cout << "Customer number:";
    cin >> n;
    customer.check_in(n);
    if (!inuse)
    {
        inuse = 1;
        begin_time = getTime();
        if (last_room == 0)
            room_bin[type] = next_room;
        else
            room[last_room].next_room = next_room;
    }
    else
        error("illegal use room!");
}

void Room::Load(string &str, unsigned long long &pointer)
{
    inuse = str[pointer++] - '0';
    type = getint_move(str, pointer);
    if (str[pointer++] != '#')
        error("Something wrong when read in the latest room(#)");
    number = getint_move(str, pointer);
    if (str[pointer++] != '#')
        error("Something wrong when read in the latest room(#)");
    begin_time = getint_move(str, pointer);
    if (str[pointer++] != '#')
        error("Something wrong when read in the latest room(#)");
    customer.Load(str, pointer);
    if (str[pointer++] != '!')
        error("Something wrong when read in the latest room(!)");
}

string Room::backStr()
{
    string ret = "";
    ret += char(inuse + '0');
    ret += int2str(type);
    ret += '#';
    ret += int2str(number);
    ret += '#';

    ret += int2str(begin_time);
    ret += '#';
    ret += customer.backStr();
    ret += '!';

    return ret;
}

ostream &operator<<(ostream &strm, const Room &obj)
{
    if (!obj.inuse)
    {
        color_printf(color_green, "%d(type:%d) is free", obj.number, obj.type);
    }
    else
    {
        color_printf(color_red, "%d(type:%d) is inuse", obj.number, obj.type);
    }
    return strm;
}

int SearchBin(int t)
{
    int p = room_bin[t];
    while (room[p].next_room != 0) // room[0].next_room=0
    {
        p = room[p].next_room;
    }
    return p;
}

void Room_Free(int x)
{
    if (room[x].inuse == 0)
        return;
    room[x].inuse = 0;
    room[x].begin_time = 0;
    room[x].customer.clear();
    int p = SearchBin(room[x].type);
    if (p == 0) // this type bin is empty
    {
        room_bin[room[x].type] = x;
    }
    else // link x to p
    {
        room[p].next_room = x;
        room[x].last_room = p;
        room[x].next_room = 0;
    }
}

int Search_Number(int x)
{
    for (int i = 1; i <= room_top; i++)
    {
        if (room[i].number == x)
            return i;
    }
    return -1;
}

void Show_All_Rooms()
{
    printf("There are %d rooms\n", room_top);
    for (int i = 1; i <= room_top; i++)
    {
        if (room[i].type == -1)
            continue;
        cout << room[i] << endl;
    }
    system("pause");
}
#endif