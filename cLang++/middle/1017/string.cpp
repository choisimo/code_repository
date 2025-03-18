//
// Created by csimo on 10/17/2024.
//
#include <iostream>
#include <string>
using namespace std;

int main()
{
    string str;
    string address("¼­¿ï½Ã ¼ººÏ±¸ »ï¼±µ¿ 389");
    string copyAddress(address);
    string address1(address);

    char text[] = {'L', 'o', 'v','e', ' ', 'C', '+', '+', '\0'};
    string title(text);

    cout << str << endl;
    cout << address << endl;
    cout << copyAddress << endl;
    cout << title << endl;
    cout << address1 << endl;
}