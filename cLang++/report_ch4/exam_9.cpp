/*
* ���� 9��.
 * Person Ŭ������ �����Ͽ� 3���� Person ��ü�� ������ �迭�� �����ϰ�, ���α׷��� �ϼ��϶�.
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
    cout << "�̸��� ��ȭ��ȣ�� �Է��Ͻÿ�. >> " << endl;

    for (int i = 0; i < 3; i++)
    {
        cout << "��ȣ [" << i << "] �Է� >> ";
        cin >> name >> tel;
        man[i].set(name, tel);
    }

    cout << "��� ����� �̸��� \n { ";
    int i = 0;
    for (; i < 3; i++)
    {
        cout << man[i].getName();
        if (i < 2)
        {
            cout << ", ";
        }
    }

    cout << " } �Դϴ�" << endl;

    bool found = false;
    int index = -1;

    do
    {
        cout << "��ȭ��ȣ�� �˻�... �̸��� �Է��Ͻÿ� >> "; cin >> name;
        cout << "�Է� ���� �̸� [" << name << "] �� �˻� ��.";

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
            cout << "\n�̸��� ã�� �� �����ϴ�! �̸��� �ٽ� Ȯ�����ּ���!" << endl;
        }
    } while (!found);


    if (index != -1)
    {
        cout << "\n [" << man[index].getName() << "] ���� ";
        cout << "��ȭ��ȣ�� " << man[index].getTel() << endl;
    }

    return 0;
}