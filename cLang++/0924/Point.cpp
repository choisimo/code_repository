//
// Created by csimo on 9/24/2024.
//
#include <iostream>
using namespace std;

class Point {
private:  int x,y;
public :
    Point();

    Point(int x, int y);

    void show() {
        cout << "(" << x << ", " << y << ")" << endl;
    }

};

Point::Point() : Point(0, 0) {}
Point::Point(int a, int b) : x(a), y(b){} // 위임 생성자


int main(){
    Point origin;
    Point target(10, 20);
    origin.show();
    target.show();
}