//
// Created by csimo on 9/12/2024.
// 2-7 예제 코드
//

#include <iostream>
#include <string>
using namespace std;


int main(){
    std::string song("Falling in love with you");
    std::string elvis("Elvis Presley");
    std::string singer;

    cout << song + " 를 부른 가수는";
    cout << "(힌트 : 첫글자는 " << elvis[0] << ")?";

    getline(cin, singer);

    if (singer == elvis) {
        cout << "맞습니다." << endl;
    } else {
        cout << "틀렸습니다." + elvis + " 입니다. " << endl;
    }
}
