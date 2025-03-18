//
// Created by csimo on 9/24/2024.
//
#include <iostream>
using namespace std;

class Rectangle{
public:
    int w,h;
    bool isSquare();
    Rectangle();
    Rectangle(int w, int h);
    Rectangle(int w);
};

Rectangle::Rectangle(){}
Rectangle::Rectangle(int w) : w(w), h(w) {}
Rectangle::Rectangle(int w, int h) : w(w), h(h) {}

bool Rectangle::isSquare() {
    cout << "width : " << w << ", height : " << h << endl;
    if (this->w && this->h) {
        return this->w == this->h;
    }
    return true;
}

int main(){
    Rectangle rect1;
    Rectangle rect2(3, 5);
    Rectangle rect3(3);

    if (rect1.isSquare()) cout << "rect1은 정사각형이다." << endl;
    if (rect2.isSquare()) cout << "rect2은 정사각형이다." << endl;
    if (rect3.isSquare()) cout << "rect3은 정사각형이다." << endl;

}
