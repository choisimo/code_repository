//
// Created by nodove on 24. 12. 10.
//
#include <iostream>
using namespace std;

class BaseMemory {
private:
    char* mem;
protected:
    BaseMemory(int size) {
        mem = new char[size];
    }
    void burnMemory(char x[], int burn) {
        for (int i = 0; i < burn; i++) {
            mem[i] = x[i];
        }
    }
    char getMemory(int x) {
        return mem[x];
    }
    void setMemory(int i, char x) {
        mem[i] = x;
    }
};

class ROM : protected BaseMemory {
private:
    int RomSize, burn;
    char x[];
public:
    ROM(int size, char x[], int burn) : BaseMemory(size) {
        this->RomSize = size;
        this->burn = burn;
        burnMemory(x, burn);
    }
    char read(int i) {
        return getMemory(i);
    }
};

class RAM : protected BaseMemory {
private:
    int RamSize;
public:
    RAM(int size) : BaseMemory(size) {
        RamSize = size;
    }
    void write(int i, char x){
        setMemory(i, x);
    }
    char read(int i) {
        return getMemory(i);
    }
};

int main() {
    char x[] = "Hello, World!";
    ROM rom(1024 * 10 , x, 13);
    RAM ram(1024 * 1024);

    for (int i = 0; i < 13; i++) {
        ram.write(i, rom.read(i));
    }
    for (int i = 0; i < 13; i++) {
        cout << rom.read(i);
    }
    for (int i = 0; i < 13; i++) {
        ram.write(i, rom.read(i));
    }
    for (int i = 0; i < 13; i++) {
        cout << ram.read(i);
    }
}