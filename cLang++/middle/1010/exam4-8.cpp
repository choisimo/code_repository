//
// Created by csimo on 10/10/2024.
//
#include <iostream>
using namespace std;

class Circle {
private:
    int radius;
public:
    Circle();

    Circle(int r);

    ~Circle();

    void setRadius(int r){
        this->radius = r;
    }

    double getArea() {
        return 3.14 * this->radius * this->radius;
    }
};

Circle::Circle(){
    this->radius = 1;
    cout << "생성자 실행 radius = " << this->radius << endl;
}

Circle::Circle(int r){
    this->radius = r;
    cout << "생성자 실행 radius = " << r << endl;
}

Circle::~Circle(){
    cout << "소멸자 실행 radius = " << this->radius << endl;
}

int main(){

    int radius;

    while (1) {
        cout << "정수 반지름 입력 (음수이면 종료) >> ";
        cin >> radius;
        if (radius < 0) break;
        Circle *p = new Circle(radius);
        cout << "원의 면적은 " << (*p).getArea();
        cout << endl;
        delete p;
    }
}