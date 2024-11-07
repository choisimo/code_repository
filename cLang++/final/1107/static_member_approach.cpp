//
// Created by nodove on 24. 11. 7.
//
#include <iostream>
using namespace std;

class PersonError {
private:
    int money;
public:

     void setMoney(int m) {
         money = m;
     }

     int getThisMoney(PersonError *ptr) {
         return (*ptr).money;
     }

};

int main() {
    PersonError errorKim;
    errorKim.setMoney(100);

    int n = errorKim.getThisMoney(&errorKim);
    cout << n << endl;
}