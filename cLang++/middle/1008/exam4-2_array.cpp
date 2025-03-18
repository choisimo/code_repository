//
// Created by csimo on 10/8/2024.
//

#include <iostream>
using namespace std;

class Circle {
    int radius;   //private
public:
    Circle() {	radius = 1; }
    Circle(int r)  { radius = r; }
    void setRadius(int r)  { radius = r; }
    double getArea();
};

double Circle::getArea() {
    return 3.14*radius*radius;
}

int main() {
    Circle circleArray[3]; 										// (1) Circle 객체 배열 생성

    // 배열의 각 원소 객체의 멤버 접근, 함수로 초기화
    circleArray[0].setRadius(10); 							// (2)
    circleArray[1].setRadius(20);
    circleArray[2].setRadius(30);

    for(int i=0; i<3; i++) // 배열의 각 원소 객체의 멤버 접근
        cout << "Circle " << i << "의 면적은 " << circleArray[i].getArea() << endl;

    Circle *p; 															// (3)
    p = circleArray; 												// (4) == &circleArray[0]
    for(int i=0; i<3; i++) { 	// 객체 포인터로 배열 접근
        cout << "Circle " << i << "의 면적은 " << p->getArea() << endl;
        p++; 															// (5)
    }
}
int main() {
    Circle circleArray[3];                                        // (1) Circle 객체 배열 생성

    // 배열의 각 원소 객체의 멤버 접근, 함수로 초기화
    circleArray[0].setRadius(10);                            // (2)
    circleArray[1].setRadius(20);
    circleArray[2].setRadius(30);

    for (int i = 0; i < 3; i++) // 배열의 각 원소 객체의 멤버 접근
        cout << "Circle " << i << "의 면적은 " << circleArray[i].getArea() << endl;


    Circle *p1 = circleArray;                                                // (4) == &circleArray[0]

    Circle *p2;
    p2 = circleArray;

    Circle *p3 = &circleArray[0];

    Circle *p4 = &(circleArray[1]);
}