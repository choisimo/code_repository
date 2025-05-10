//
// Created by nodove on 24. 12. 10.
//
#include <iostream>
#include <vector>
using namespace std;

class Point {
private:
    int x,y;
public:
    Point(int x, int y) : x(x), y(y) {}
    int getX() { return x; }
    int getY() { return y; }
protected:
    void move(int x, int y) {
        this->x += x;
        this->y += y;
    }
    void print() {
            cout << "   ^   ^   " << endl;
            cout << "  ( o   o )" << endl;
            cout << "    \\ ^ /  " << endl;
            cout << "    |___|  " << endl;
            cout << "   /     \\ " << endl;
            cout << "  |       |" << endl;
            cout << "   \\_____/ " << endl;
    }
};

class ColorPoint : public Point {
private:
    int x, y;
    string color;
public:
    ColorPoint(int x, int y, string color) : Point(x, y)
    {
        this->x = x;
        this->y = y;
        this->color = color;
    }

    void setPoint(int x, int y) {
        this->x = x;
        this->y = y;
        move(this->x, this->y);
    }

    void setColor(string color) {
        this->color = color;
    }

    void show() {
        cout << color << "색으로 (" << getX() << ", " << getY() << ") 에 위치한 점 입니다." << endl;
    }
};

int main() {
    ColorPoint cp(5, 5, "Red");
    cp.setPoint(10, 20);
    cp.setColor("BLUE");
    cp.show();
}