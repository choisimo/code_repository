//
// Created by nodove on 24. 11. 9.
//
//
// Created by nodove on 24. 11. 9.
//


/*
 * 11-3.
 * */

#include <iostream>
#include <cstring>
using namespace std;

class Book {
private:
    char* title;
    int price;
public:
    Book(const char* title, int price) {
        this->price = price;
        this->title = new char[strlen(title) + 1];
        strcpy(this->title, title);
    }

    Book(const Book& book) {
        this->price = book.price;
        this->title = new char[strlen(book.title) + 1];
        strcpy(this->title, book.title);
    }

    ~Book(){
        delete[] title;
    };

    void set(const char *title, int price);

    void show() {
        cout << title << ": " << "원" << endl;
    }
};

void Book::set(const char* title, int price) {
    if (this->title){
        delete[] this->title;
    }
    this->price = price;
    this->title = new char[strlen(title) + 1];
    strcpy(this->title, title);
}

int main() {
    Book cpp("명품 C++", 10000);
    Book java = cpp;
    java.set("명품자바", 12000);
    cpp.show();
    java.show();
}
