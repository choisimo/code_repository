//
// Created by csimo on 9/12/2024.
//

/*
 * ch 2 - no 10
 * 문자열을 하나 입력받고 문자열의 부분 문자열을
 * 다음과 같이 출력하는 프로그램을 작성하라.
 * */


#include <iostream>
#include <cstring>
#include <string>
using namespace std;

class getCount{
private:
public:
};
int main() {
    string strValue;
    int n;
    // char -> cin.getline(char 배열, 사이즈, \n <- enter?);
    getline(cin, strValue);

    while(true)
    {
        if (strcmp(strValue[n],'\n') == 0){
            cout << strValue[n] << " is enter? " << endl;
            break;
        }

        for (int i = 0; i < n; i++){
            cout << "count[" <<  n  << "]" << strValue[n] << endl;
        }

        break;
    }
    return 0;
}

