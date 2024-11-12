//
// Created by nodove on 24. 11. 12.
//

/*
 * Book 클래스의 생성자, 소멸자, set() 함수를 작성하여 코드를 완성하라.
 * (깊은 복사 생성자 추가하여) string 버전만 작성하라.
 * */

#include <iostream>
#include <cstring>
using namespace std;

class Book {
private:
    string title;
    int price;
public:
    Book(string title, int price) {
        this->price = price;
        this->title = title;
    }

    Book(Book& book) {
        this->price = book.price;
        this->title = book.title;
    }

    ~Book(){}

    void set(string title, int price);

    void show() {
        cout << this->title << ": " << this->price << "원" << endl;
    }
};

void Book::set(string title, int price) {
    this->price = price;
    this->title = title;
}

int main() {
    Book cpp("명품 C++", 10000);
    Book java = cpp;
    java.set("명품자바", 12000);
    cpp.show();
    java.show();
}
