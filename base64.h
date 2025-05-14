/*
 * Base64 encoding and decoding library.
 * 
 * Features:
 * - Encodes a given string into Base64 format.
 * - Decodes a Base64 encoded string back to its original form.
 * 
 * Functions:
 * - base64_encode(string s): Encodes the input string using Base64.
 * - base64_decode(string s): Decodes the Base64 encoded string.
 * 
 * Usage:
 * - Include this header file and call the functions with appropriate strings.
 * 
 * Note:
 * - The encoding table used here is a custom table. Ensure consistency between encoding and decoding tables.
 */
#ifndef BASE64_H
#define BASE64_H
#include <bits/stdc++.h>
using namespace std;
const string table = "0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm/+";
// const string table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
string base64_encode(string s)
{
    string result;
    int val = 0, valb = -6;
    for (char c : s)
    {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            result.push_back(table[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
        result.push_back(table[((val << 8) >> (valb + 8)) & 0x3F]);
    while (result.size() % 4)
        result.push_back('=');
    return result;
}

string base64_decode(string s)
{
    vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
        T[table[i]] = i;
    string result;
    int val = 0, valb = -8;
    for (char c : s)
    {
        if (T[c] == -1)
            break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0)
        {
            result.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return result;
}
#endif