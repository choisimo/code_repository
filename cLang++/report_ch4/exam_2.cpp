/*
 * ���� 2��.
 * ���� ���� 5���� ���� �迭�� �Ҵ� �ް�, ������ 5�� �Է¹޾�
 * ����� ���ϰ� ����� �� �迭�� �Ҹ��Ű���� main() �Լ� �ۼ�
 * @author : simon choi
 * @date   : 24-10-29
 */


#include <iostream>
using namespace std;

int main()
{
    int *numbers = new int[5];
    int sum = 0;
    cout << "���� 5�� �Է� >> ";

    for (int i = 0; i < 5; i++)
    {
        cin >> numbers[i];
        sum += numbers[i];
    }

    cout << "��� : " << (double) sum / 5 << endl;
    delete[] numbers;
}
