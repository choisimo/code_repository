//
// Created by csimo on 9/28/2024.
//


/*
 * Oval Ŭ������ �־��� �簢���� �����ϴ� Ÿ���� �߻�ȭ�� Ŭ�����̴�. Oval Ŭ���� ����� ��� ������ ����.
 *
 * ���� ���� �簢�� �ʺ�� ���̸� ������ width, height ��� ����
 * �ʺ�� ���� ���� �Ű� ������ �޴� ������
 * �ʺ�� ���̸� 1�� �ʱ�ȭ�ϴ� �Ű� ���� ���� ������
 * width �� height �� ����ϴ� �Ҹ���
 *
 * // Ÿ���� �ʺ� �����ϴ� getWidth() �Լ� ���
 * Ÿ���� ���̸� �����ϴ� getHeight() �Լ� ���
 * Ÿ���� ������ �����ϴ� getArea() �Լ� ���
 * //
 * Ÿ���� �ʺ�� ���̸� �����ϴ� set(int w, int h) �Լ� ���
 * Ÿ���� �ʺ�� ���̸� ȭ�鿡 ����ϴ� show() �Լ� ���
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

    // Ÿ���� �ʺ� ����
    int getWidth();

    // Ÿ���� ���̸� ����
    int getHeight();

    // Ÿ���� ������ ���
    double getArea();

    // Ÿ���� �ʺ�� ���̸� ����
    void set(int w, int h);

    // Ÿ���� �ʺ�� ���̸� ���
    void show();
};

Oval::Oval(int w, int h) : width(w), height(h){}
Oval::Oval() : width(1), height(1){}
Oval::~Oval(){
    cout << "~�Ҹ��� ȣ��" << endl;
    cout << "width: " << this->width << "\n" << "height: " << this->height << endl;
    cout << "~�Ҹ��� ����" << endl;
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
