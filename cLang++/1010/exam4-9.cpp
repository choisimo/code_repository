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
    radius = 1;
    cout << "积己磊 角青 radius = " << this->radius << endl;
}

Circle::Circle(int r){
    this->radius = r;
    cout << "积己磊 角青 radius = " << r << endl;
}

Circle::~Circle(){
    cout << "家戈磊 角青 radius = " << this->radius << endl;
}

int main(){
    Circle *pArr = new Circle[3];

    for (int i = 0; i < 3; i++) {
            pArr[i].setRadius((i + 1)* 10);
        //(*(pArr++)).setRadius((++i) * 10);
    }

    for (int i = 0; i < 3; i++) {
        cout << pArr[i].getArea() << endl;
    }

    Circle *p = pArr;
    for (int i = 0; i < 3; i++) {
        cout << (*(p++)).getArea() << endl;
    }

    delete[] pArr;

}