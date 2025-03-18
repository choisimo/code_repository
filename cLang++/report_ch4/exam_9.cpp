/*
* 문제 9번.
 * Person 클래스를 참고하여 3개의 Person 객체를 가지는 배열을 선언하고, 프로그램을 완성하라.
 * @author : simon choi
 * @date   : 24-10-29
 */


#include <iostream>
#include <string>
using namespace std;

class Person {
private:
    string name;
    string tel;
public:
    Person();
    string getName() {return name;}
    string getTel() {return tel;}
    void set(string name, string tel);
};

Person::Person()
{
    name[0] == NULL; tel[0] = NULL;
}

void Person::set(string name, string tel)
{
    this->name = name;
    this->tel = tel;
}

int main()
{
    Person man[3];
    string name, tel;
    cout << "이름과 전화번호를 입력하시오. >> " << endl;

    for (int i = 0; i < 3; i++)
    {
        cout << "번호 [" << i << "] 입력 >> ";
        cin >> name >> tel;
        man[i].set(name, tel);
    }

    cout << "모든 사람의 이름은 \n { ";
    int i = 0;
    for (; i < 3; i++)
    {
        cout << man[i].getName();
        if (i < 2)
        {
            cout << ", ";
        }
    }

    cout << " } 입니다" << endl;

    bool found = false;
    int index = -1;

    do
    {
        cout << "전화번호를 검색... 이름을 입력하시오 >> "; cin >> name;
        cout << "입력 받은 이름 [" << name << "] 을 검색 중.";

        for (int i = 0; i < 3; i++)
        {
            cout << ".";
            if (man[i].getName() == name)
            {
                index = i;
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "\n이름을 찾을 수 없습니다! 이름을 다시 확인해주세요!" << endl;
        }
    } while (!found);


    if (index != -1)
    {
        cout << "\n [" << man[index].getName() << "] 님의 ";
        cout << "전화번호는 " << man[index].getTel() << endl;
    }

    return 0;
}