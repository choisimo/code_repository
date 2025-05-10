//
// Created by csimo on 3/21/2024.
//

#include <iostream>
using namespace std;
int main(){
    int *t = (int *) malloc(sizeof(int) * 5);
    int *p = new int[999999999]{1,2,3,4,5};
    p[0] = 1;
    int count = 0;
    for(int i = 0; i < 999999999; i++){
        count++;
        p[i] = count;
        cout << p[i] << endl;
    }
    return 0;
}