//
// Created by nodove on 24. 11. 5.
//
#include <iostream>
#include <cstring>
using namespace std;

class Person {
    char* name;
    int id;
public:
    Person(int id, const char* name); // 생성자
    Person(const Person& p);          // 복사 생성자
    ~Person(); // 소멸자
    void changeName(const char* name); // 이름 변경
    void show() const {
        cout << id << ',' << name << endl;
    } // 이름과 id 출력
};

Person::Person(int id, const char *name) {
    this->id = id;
    int len = strlen(name); // name의 문자열 길이
    this->name = new char[len+1]; // name 문자열 공간 할당
    strcpy(this->name, name); // name 문자열 복사
}
Person::Person(const Person &p) {
    this->id = p.id;
    int len = strlen(p.name); // p.name의 문자열 길이
    this->name = new char[len+1]; // p.name 문자열 공간 할당
    strcpy(this->name, p.name); // p.name 문자열 복사
    cout << "복사 생성자 실행. 원본 객체의 이름 : " << this->name << endl;
}

Person::~Person() {
    if (name)          // 만약 name에 동적 메모리가 할당되어 있으면
        delete[] name; // name 메모리 반환
}

void Person::changeName(const char *name) {
    if (strlen(name) > strlen(this->name))
        return; // 변경하지 않음
    strcpy(this->name, name); // name 변경

}

int main(){
    Person father(1, "Kitae");
    Person daughter(father); // 복사 생성자 호출
    cout << "daughter 객체 생성 직후 ----" << endl;
    father.show();
    daughter.show();

    daughter.changeName("Grace");
    cout << "daughter 이름을 Grace로 변경한 후 ----" << endl;
    father.show();
    daughter.show();

    return 0;
}