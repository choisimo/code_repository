//
// Created by csimo on 3/21/2024.
//

#include <iostream>

using namespace std;

class Rectangle{
public:
    void printRectangle(int n){
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                cout << "*";
            }
            cout << endl;
        }
    }
};
int main(){
    int n = 0;
    //cout << "정사각형 한 변의 길이 n(1<=n<=100) 을 입력하시오." << endl;
    cin >> n;
    Rectangle *Rec = new Rectangle();
    Rec->printRectangle(n);
    //(*Rec).printRectangle(n);
    delete Rec;
    return 0;
}