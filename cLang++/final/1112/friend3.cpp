//
// Created by nodove on 24. 11. 12.
//

#include <iostream>
using namespace std;


class Rect;

class RectManager {
public:
    bool equals(Rect r, Rect s);
    void copy(Rect& dest, Rect& src);
};

class Rect{
    int width, height;
public:
    Rect(int w, int h) : width(w), height(h) {}
    friend RectManager;
};

bool RectManager::equals(Rect r, Rect s) {
    if (r.width == s.width && r.height == s.height) return true;
    else return false;
}

void RectManager::copy(Rect &d, Rect &s) {
    d.width = s.width;
    d.height = s.height;
}

int main() {
    Rect a(3, 4), b(5, 6);
    RectManager man;

    man.copy(b, a);
    if (man.equals(a, b)) {
        cout <<  "equal" << endl;
    } else {
        cout << "not equals" << endl;
    }
}