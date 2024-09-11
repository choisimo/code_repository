//
// Created by csimo on 9/10/2024.
//
#include <iostream>

double area(int r); // 함수의 원형 선언

/*
 * area 함수 구현
 * @param return 반지름 r의 원 면적
 * */
double area(int r){
    return 3.14 * r * r;
}

int main(){

    int n = 3;
    char c = '#';
    std::cout << c << 5.5 << '-' << n << "hello" << true << std::endl;
    std::cout << "n + 5 = " << n + 5 << '\n';
    std::cout << "면적은 " << area(n); // 함수의 area()의 리턴 값 출력
}