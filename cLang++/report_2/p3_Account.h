//
// Created by csimo on 9/28/2024.
//

#ifndef P3_ACCOUNT_H
#define P3_ACCOUNT_H

#include <string>

struct accountInfo {
    std::string owner;
    int id;
    int money;

    accountInfo(std::string owner, int id, int money) : owner(owner), id(id), money(money){}
};

#endif //P3_ACCOUNT_H
