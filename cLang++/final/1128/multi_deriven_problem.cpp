//
// Created by nodove on 24. 11. 28.
//
#include <iostream>
using namespace std;

class BaseId {
public:
    int mode;
};

class In : public BaseId {
public:
    int readPos;
};

class Out : public BaseId {
public:
    int writePos;
};

class InOut : public In, public Out {
public:
    bool safe;
};

int main(){
    InOut ioObj;

    ioObj.readPos = 10;
    ioObj.writePos = 20;
    ioObj.safe = true;
    ioObj.mode = 5; // error: 'BaseId' has no member named 'mode'
}