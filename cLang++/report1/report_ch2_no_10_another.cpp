#include <iostream>
#include <string>
using namespace std;

int main() {
    string strValue;
    cout << "Enter a string: ";
    getline(cin, strValue);  // Reading the whole line as input

    // Printing all substrings starting at each position
    for (int i = 0; i < strValue.length(); i++) {
        for (int j = i; j < strValue.length(); j++) {
            cout << strValue.substr(i, j - i + 1) << endl;
        }
    }

    return 0;
}
