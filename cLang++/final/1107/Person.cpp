//
// Created by nodove on 24. 11. 7.
//
#include<iostream>
using namespace std;

class Person {
public:
    int money;
    void addMoney(int m) {
        this->money += m;
    }

    static int sharedMoney; // 공금
    static void addShared(int m) {
        sharedMoney += m;
    }
};

// static 변수 공간 할당, 프로그램의 전역 공간에 할당
int Person::sharedMoney = 10; // sharedMoney 를 10으로 초기화

int main(){
    Person::addShared(50);
    cout << Person::sharedMoney << endl;

    Person han;
    han.money = 100;
    han.sharedMoney = 200;
    Person::sharedMoney = 300;
    Person::addShared(100);

    cout << han.money << ' ' << Person::sharedMoney << endl;
}