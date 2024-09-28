//
// Created by csimo on 9/28/2024.
//

/*
 * 1. Tower 클래스를 작성하라.
 * */

#include <iostream>
using namespace std;

class Tower
{
private:
    int height;
public:
    Tower() : height(1) {}
    Tower(int height);
    int getHeight();
};

Tower::Tower(int height) : height(height){}

int Tower::getHeight() { return this->height; }

int main(){
    Tower myTower;
    Tower seoulTower(100);
    cout << "높이는 " << myTower.getHeight() << "미터" << endl;
    cout << "높이는 " << seoulTower.getHeight() << "미터" << endl;
}