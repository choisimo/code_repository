//
// Created by csimo on 9/28/2024.
//

/*
 * 1. Tower Ŭ������ �ۼ��϶�.
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
    cout << "���̴� " << myTower.getHeight() << "����" << endl;
    cout << "���̴� " << seoulTower.getHeight() << "����" << endl;
}