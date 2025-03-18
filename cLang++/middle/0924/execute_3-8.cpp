//
// Created by csimo on 9/24/2024.
//
#include <iostream>
using namespace std;

class Circle {
public:
    int radius;

    Circle();

    Circle(int r);

    ~Circle();

    double getArea();
};

Circle::Circle(){
    this->radius = 1;
    cout << "������ " << this->radius << " �� ����" << endl;
}


Circle::Circle(int r){
    this->radius = r;
    cout << "������ " << this->radius << " �� ����" << endl;
}


Circle::~Circle(){
    cout << "������ " << this->radius << " �� �Ҹ�" << endl;
}

double Circle::getArea() { return 3.14 * this->radius * this->radius; }

Circle globalDonut(1000);
Circle globalPizza(2000);


void f(){
    Circle fDonut(1000);
    Circle fPizza(200);
}

int main(){
    Circle mainDonut();
    Circle mainPizza(30);
    f();
}







