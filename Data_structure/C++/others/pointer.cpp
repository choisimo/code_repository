#include <iostream>
#include <string>

using namespace std;

struct what {
  int id;
  string name;
  what(int id, const string* name){
    cout << "name 의 값은 : " << *name << endl;
    this->id =  id;
    this->name = *name;
  }
};

int main() {
    string str = "123";
    string *str_mem = &str;

    cout << "str_mem 주소 값 : " << str_mem << endl;
    cout << "str_mem 포인터 변수의 주소 : " 
    << &str_mem << endl;
    what myWhat(1, &str); // 수정된 코드
    what myWhat2(myWhat);
    return 1;
}
