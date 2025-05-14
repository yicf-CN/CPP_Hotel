/*
 * Customer class definition.
 * 
 * Features:
 * - Manages customer information for hotel rooms.
 * - Supports adding, clearing, and listing customer names.
 * - Provides methods for loading and saving customer data in a specific format.
 * 
 * Methods:
 * - check_in(int n): Prompts the user to enter names for n customers.
 * - Customer_Add(string s): Adds a customer name to the list.
 * - clear(): Clears all customer data.
 * - list(): Returns a string containing all customer names.
 * - Load(string &str, unsigned long long &pointer): Loads customer data from a string.
 * - backStr(): Returns a string representation of the customer data for saving.
 * 
 * Usage:
 * - Instantiate the Customer class and use its methods to manage customer information.
 * 
 * Note:
 * - The class assumes that customer names do not contain illegal characters (e.g., ':').
 * - The Load and backStr methods use a specific format for storing and retrieving data.
 */
#ifndef customer_H
#define customer_H
class Customer
{
    int cnt;
    vector<string> name;

public:
    Customer()
    {
        cnt = 0;
        name.resize(0);
    }
    void check_in(int n);
    void Customer_Add(string s)
    {
        cnt++;
        name.push_back(s);
    }
    void clear()
    {
        cnt = 0;
        name.clear();
    }
    string list();
    void Load(string &str, unsigned long long &pointer); //{cnt}:{name[1]}:{name[cnt]}
    string backStr();
    friend ostream &operator<<(ostream &, const Customer &);
};

void Customer::check_in(int n)
{
    cin.ignore();
    name.resize(n + 1);
    cnt = n;
    string s;
    bool illegal_name = 0;
    for (int i = 1; i <= n; i++)
    {
        illegal_name = 0;
        cout << "name " << i << ":";
        getline(cin, s);
        for (char c : s)
        {
            if (!isletter(c) || c == ':')
            {
                cout << "Illegal letter:" << c << endl;
                cout << "Try again" << endl;
                illegal_name = 1;
                break;
            }
        }
        if (illegal_name)
        {
            i--;
            continue;
        }
        name[i] = s;
    }
}

string Customer::list()
{
    string ret = "";
    for (int i = 1; i <= cnt; i++)
    {
        ret += name[i];
        if (i < cnt)
            ret += '\n';
    }
    return ret;
}

void Customer::Load(string &str, unsigned long long &pointer) //{cnt}:{name[1]}:{name[cnt]}
{
    cnt = getint_move(str, pointer);
    if (str[pointer++] != ':' && cnt != 0)
        error("Something wrong when read in the latest room(:)");
    if (cnt == 0)
        pointer--;
    Customer_Add("");
    cnt--;
    for (int i = 1; i <= cnt; i++)
    {
        string tmp_name = "";
        for (; str[pointer] != ':' && str[pointer] != '!'; pointer++)
        {
            tmp_name += str[pointer];
        }
        if (str[pointer] != ':' && str[pointer] != '!')
            error("Something wrong when read in the latest room(:)");
        if (str[pointer] == ':')
            pointer++;
        Customer_Add(tmp_name);
        cnt--;
    }
}

string Customer::backStr()
{
    string ret = "";
    ret += int2str(cnt);
    for (int i = 1; i <= cnt; i++)
    {
        ret += ':';
        ret += name[i];
    }
    return ret;
}

ostream &operator<<(ostream &strm, const Customer &obj)
{
    for (int i = 1; i <= obj.cnt; i++)
    {
        strm << "{" << obj.name[i] << "}";
        if (i < obj.cnt)
            strm << ":";
    }
    return strm;
}
#endif