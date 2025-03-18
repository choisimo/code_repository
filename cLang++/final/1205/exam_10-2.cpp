//
// Created by nodove on 24. 12. 5.
//
#include <iostream>
using namespace std;

template<typename T>
T bigger(T a, T b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int main(){
    int a = 20, b = 50;
    char c = 'A', d = 'Z';
    cout << bigger(a, b) << endl;
    cout << bigger(c, d) << endl;
    return 0;

}