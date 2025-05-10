//
// Created by csimo on 9/28/2024.
//


/*
 *      // console //
 * ����������������������������������������������
 * �� Kitae�� �ܾ��� 55000  ��
 * �� Kitae�� �ܾ��� 35000  ��
 * ����������������������������������������������
 * */


#include <iostream>
#include "p3_Account.h"
using namespace std;

class Account {
private:
    accountInfo aInfo;
public:
    Account(std::string name, int id, int money) : aInfo(name, id, money){}

    void deposit(int amount);

    int withdraw(int amount);
    std::string getOwner() const;
    int inquiry() const;
};

void Account::deposit(int amount) {
    this->aInfo.money += amount;
}

int Account::withdraw(int amount) {
    if (this->aInfo.money >= amount) {
        this->aInfo.money -= amount;
        return this->aInfo.money;
    } else {
        throw new runtime_error("withdraw failed.. due to leakage of balance");
    }
}

std::string Account::getOwner() const {
    return this->aInfo.owner;
}

int Account::inquiry() const {
    return this->aInfo.money;
}


int main() {
    Account a("Kitae", 1, 5000);
    a.deposit(50000);
    cout << a.getOwner() << "�� �ܾ��� " << a.inquiry() << endl;
    int money = a.withdraw(20000);
    cout << a.getOwner() << "�� �ܾ��� " << a.inquiry() << endl;
}