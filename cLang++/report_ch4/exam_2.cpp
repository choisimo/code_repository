/*
 * 문제 2번.
 * 정수 공간 5개를 동적 배열로 할당 받고, 정수를 5개 입력받아
 * 평균을 구하고 출력한 뒤 배열을 소멸시키도록 main() 함수 작성
 * @author : simon choi
 * @date   : 24-10-29
 */


#include <iostream>
using namespace std;

int main()
{
    int *numbers = new int[5];
    int sum = 0;
    cout << "정수 5개 입력 >> ";

    for (int i = 0; i < 5; i++)
    {
        cin >> numbers[i];
        sum += numbers[i];
    }

    cout << "평균 : " << (double) sum / 5 << endl;
    delete[] numbers;
}
