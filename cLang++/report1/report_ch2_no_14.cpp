//
// Created by csimo on 9/12/2024.
//

/*
 * 커피를 주문하는 간단한 C++ 프로그램을 작성해보자.
 * 커피 종류는 “에스프레소”, “아메리카노”, “카푸치노”의 3가지이며 가격은 각각 2000원, 2300원, 2500원이다.
 * 하루에 20000원 이상 벌게 되면 카페를 닫는다. 실행 결과와 같이 작동하는 프로그램을 작성하라.
* */

#include <iostream>
#include <string>
using namespace std;

class cafe {
private:
    int menu_no;
    int price;
    char *menu[];
public:
    cafe(int m, int p, string n) : menu_no(m), price(p), menu(n)
    {}
};
int main() {

}