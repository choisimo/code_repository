//
// Created by nodove on 24. 11. 9.
//


/*
 * 11-1. Book 클래스의 생성자, 소멸자, set() 함수를 작성하라.
 * set() 함수는 멤버 변수 title에 할당된 메모리가 있으면 먼저 반환한다.
 * 그러고 나서 새로운 메모리를 할당받고 이곳에 매개 변수로 전달받은 책 이름을 저장한다.
 * [생성자, 소멸자, set()함수]
 * */


#include <iostream>
#include <cstring>
using namespace std;

class Book {
private:
    char* title;
    int price;
public:
    Book(char* title, int price) {
        this->price = price;
        this->title = new char[strlen(title) + 1];
        strcpy(this->title, title);
    }

    ~Book(){
        delete[] title;
    };

    void set(char *title, int price);
};

void Book::set(char* title, int price) {
    if (this->title){
        delete[] this->title;
    }
    this->price = price;
    this->title = new char[strlen(title) + 1];
    strcpy(this->title, title);
}
