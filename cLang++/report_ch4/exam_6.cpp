/*
* ���� 6��.
 * string Ŭ������ �̿��Ͽ� ����ڰ� �Է��� ���� �� ���� ���ڿ���
 * �Է¹ް� �Ųٷ� ����ϴ� ���α׷��� �ۼ��Ͻÿ�.
 * @author : simon choi
 * @date   : 24-10-29
 */

#include <cstring>
#include <iostream>
using namespace std;

int main() {
    string str;
    cout << "�Ʒ��� ���� �� ���� �Է� (exit �Է� �� ����) : " << endl;

    while (true)
    {
        cout << " >> ";
        getline(cin, str, '\n');
        if (str == "exit") break;

        for (int i = str.size(); i >= 0; i--)
        {
            cout << str[i];
        } cout << endl;
    }
}