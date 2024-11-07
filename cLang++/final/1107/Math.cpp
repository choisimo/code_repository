//
// Created by nodove on 24. 11. 7.
//
#include <iostream>
using namespace std;

class Math {
public:
    static int abs(int a) {
        return a > 0 ? a : -a;
    }
    static int max(int a, int b) {
        return a > b ? a : b;
    }
    static int min(int a, int b) {
        return a < b ? a : b;
    }
};

int main(){
    cout << Math::abs(-5) << endl;
    cout << Math::max(10, 20) << endl;
    cout << Math::min(-10, 20) << endl;

    cout << Math::abs(5) << endl;
    cout << Math::max(20, 10) << endl;
    cout << Math::min(20, -10) << endl;

    return 0;
}