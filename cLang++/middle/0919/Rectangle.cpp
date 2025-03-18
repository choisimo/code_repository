//
// Created by csimo on 9/19/2024.
//
#include <iostream>
using namespace std;

class Rectangle{
public:
    int width;
    int height;

    int getArea() const;

    int getCircu() const{
        return 2 * (this->width + this->height);
    }
};

int Rectangle::getArea() const{
    return this->width * this->height;
}


int main(){
    Rectangle rect;
    rect.width = 3;
    rect.height = 5;

    cout << "사각형의 면적은 " << rect.getArea() << endl;
    cout << "사각형의 둘레는 " << rect.getCircu() << endl;
}