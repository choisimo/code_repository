//
// Created by csimo on 9/12/2024.
// 2�� ����Ʈ 3�� ����
//

#include <iostream>
using namespace std;

int main() {
    int n1, n2;
    cout << "�� ���� �Է��϶�>>";
    cin >> n1 >> n2;

    int bignum = (n1 > n2 ? n1 : n2);
    cout << "ū�� = " << bignum << endl;
}