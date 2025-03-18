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

    cout << "�簢���� ������ " << rect.getArea() << endl;
    cout << "�簢���� �ѷ��� " << rect.getCircu() << endl;
}