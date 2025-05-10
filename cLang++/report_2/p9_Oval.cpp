//
// Created by csimo on 9/28/2024.
//


/*
 * Oval 클래스는 주어진 사각형에 내접하는 타원을 추상화한 클래스이다. Oval 클래스 멤버는 모두 다음과 같다.
 *
 * 정수 값의 사각형 너비와 높이를 가지는 width, height 멤버 변수
 * 너비와 높이 값을 매개 변수로 받는 생성자
 * 너비와 높이를 1로 초기화하는 매개 변수 없는 생성자
 * width 와 height 를 출력하는 소멸자
 *
 * // 타원의 너비를 리턴하는 getWidth() 함수 멤버
 * 타원의 높이를 리턴하는 getHeight() 함수 멤버
 * 타원의 면적을 리턴하는 getArea() 함수 멤버
 * //
 * 타원의 너비와 높이를 변경하는 set(int w, int h) 함수 멤버
 * 타원의 너비와 높이를 화면에 출력하는 show() 함수 멤버
 */


#include <iostream>
#include <cmath>
using namespace std;

class Oval{
private:
    int width;
    int height;
public:
    Oval(int w, int h);
    Oval();
    ~Oval();

    // 타원의 너비를 리턴
    int getWidth();

    // 타원의 높이를 리턴
    int getHeight();

    // 타원의 면적을 출력
    double getArea();

    // 타원의 너비와 높이를 변경
    void set(int w, int h);

    // 타원의 너비와 높이를 출력
    void show();
};

Oval::Oval(int w, int h) : width(w), height(h){}
Oval::Oval() : width(1), height(1){}
Oval::~Oval(){
    cout << "~소멸자 호출" << endl;
    cout << "width: " << this->width << "\n" << "height: " << this->height << endl;
    cout << "~소멸자 종료" << endl;
}


int Oval::getWidth() {
    return this->width;
}
int Oval::getHeight() {
    return this->height;
}
double Oval::getArea() { return M_PI * (width / 2.0) * (height / 2.0); }

void Oval::set(int w, int h) {
    this->width = w;
    this->height = h;
}
void Oval::show() {
    cout << "width: " << this->width << "height: " << this->height << endl;
}

int main(){
    Oval oval(10, 20);
    oval.show();
    cout << "Area: " << oval.getArea() << endl;

    Oval oval1;
    oval1.show();
    oval1.set(15,30);
    oval1.show();
    cout << "Area: " << oval1.getArea() << endl;
    return 0;
}
