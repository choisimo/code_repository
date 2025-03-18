//
// Created by csimo on 9/26/2024.
//

#include <iostream>
using namespace std;

struct StructCircle{
private:
    int radius;
public :
    StructCircle(int r)
    {
        radius = r;
    }
        double getArea();
};

double StructCircle::getArea() {
    return 3.14 * radius * radius;
}
x
int main(){
    StructCircle waffle(3);
    cout << "면적은 " << waffle.getArea();
}


// C++ struct 는 멤버함수 넣을 수 있다

