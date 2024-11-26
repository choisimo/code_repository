//
// Created by nodove on 24. 11. 26.
//
#include <iostream>
#include <string>
using namespace std;

class TV {
private:
    int size; // TV의 크기
public:
    TV() {
        size = 20;
    }
    TV(int size) {
        this->size = size;
    }
    int getSize() {
        return this->size;
    }
};

class WideTV : public TV {
    bool videoIn; // TV에 비디오 입력이 가능한지 여부
public:
    WideTV(int size, bool videoIn) : TV(size) {
        this->videoIn = videoIn;
    }
    bool getVideoIn() {
        return this->videoIn;
    }
};

class SmartTv : public WideTV {
    string ipAddr; // TV의 IP 주소
public:
    SmartTv(string ipAddr, int size) : WideTV(size, true){
        this->ipAddr = ipAddr;
    }
    string getipAddr() {
        return this->ipAddr;
    }
};


int main() {
    SmartTv htv("192.168.0.1", 32);
    cout << "TV size: " << htv.getSize() << endl;
    cout << "Video input: " << htv.getVideoIn() << endl;
    cout << "IP address: " << htv.getipAddr() << endl;
    return 0;
}