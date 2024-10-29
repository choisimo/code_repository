//
// Created by csimo on 10/8/2024.
//
#include <iostream>
using namespace std;

class Circle{
private:
    int radius;
public:
    Circle(){
        this->radius = 1;
    }
    Circle(int r) {
        this->radius = r;
    }
    void setRadius(int r) {
        this->radius = r;
    }

    double getArea();
};

double Circle::getArea(){
    return 3.14 * this->radius * this->radius;
}

int main(){
    Circle circleArray[3] = {
            Circle(10), Circle(20), Circle(30)
    };

    // Circle *ptr = circleArray;
    Circle *ptr = &circleArray[0];

    for (int i = 0; i < 3; i++) {
        // cout << "Circle " << i << "의 면적은" << ptr->getArea() << endl;
        cout << "Circle " << i << "의 면적은 " << (*ptr).getArea() << endl;
        ptr = &circleArray[i];
        // ptr++;
    }


}