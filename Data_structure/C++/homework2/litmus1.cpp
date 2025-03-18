//
// Created by csimo on 4/16/2024.
//
#include <iostream>
using namespace std;

class circulate {
private:
    int *sum = new int[4];
    int num_1 = 0;
    int num_2 = 0;

public :
    explicit circulate(int n1, int n2) : num_1(n1), num_2(n2) {
        result();
    }

    ~circulate() {
        delete[] sum;
    };

    void result() {
        sum[0] = num_1 + num_2;
        sum[1] = num_1 - num_2;
        sum[2] = num_1 * num_2;
        sum[3] = num_1 / num_2;
    }

    void printResult(){
        for (int i = 0; i < 4; i++){
            cout << sum[i] << " ";
        }
    }
};

int main(int argc, char* argv[]){
    int n1, n2;
    cin >> n1 >> n2;
    while (n2 == 0){
        cin >> n2;
    }
    circulate cir(n1, n2);
    cir.printResult();

    return 0;
}