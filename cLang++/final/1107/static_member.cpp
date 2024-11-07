//
// Created by nodove on 24. 11. 7.
//
class Person {
public:
    int money;
    void addMoney(int m) {
        this->money += m;
    }

    static int sharedMoney; // 공금
    static void addSharedMoney(int m) {
        sharedMoney += m;
    }
};

// static 변수 공간 할당, 프로그램의 전역 공간에 할당
int Person::sharedMoney = 10; // sharedMoney 를 10으로 초기화