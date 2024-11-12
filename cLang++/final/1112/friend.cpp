//
// Created by nodove on 24. 11. 12.
//

#include <iostream>
using namespace std;

class Rect;
bool equals(Rect r, Rect s);

class Rect{
    int width, height;
public:
    Rect(int w, int h) : width(w), height(h) {}
    friend bool equals(Rect r, Rect s);
};

bool equals(Rect r, Rect s){
    if(r.width == s.width && r.height == s.height)
        return true;
    else
        return false;
}

int main(){
    Rect a(3, 4), b(3, 4);
    if(equals(a, b))
        cout << "equal" << endl;
    else
        cout << "not equal" << endl;
    return 0;
}