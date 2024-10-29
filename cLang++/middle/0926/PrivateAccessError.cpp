//
// Created by csimo on 9/26/2024
// .
//

#include <iostream>
using namespace std;

class PrivateAccessError {
public:
    int a;

    void f();

    PrivateAccessError();

public:
    int b;

    PrivateAccessError(int x);

    void g();
};

PrivateAccessError::PrivateAccessError() {
    a = 1;
    b = 1;
}

PrivateAccessError::PrivateAccessError(int x) {
    a = x;
    b = x;
}

void PrivateAccessError::f() {
    a = 5;
    b = 5;
}

void PrivateAccessError::g(){
    a = 6;
    b = 6;
}

int main(){
    PrivateAccessError objA;
    PrivateAccessError objB(100);
    objB.a = 10;
    objB.b = 20;
    objB.f();
    objB.g();
}


/*
 * 생성자 PrivateAccessError() 는 private 이므로 main 함수에서 호출 불가능하다.
 * 이하, a 및 f() 는 같은 이유로 접근 불가능 하다.
 * */