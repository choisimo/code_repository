#include <iostream>

using namespace std;

int main(){
    int *arr = new int[10]{1,2,3,4,5,6};

    int* int_ptr = nullptr;
    int_ptr = new int;

    *int_ptr = 100;
    cout << *int_ptr << endl;

    if (int_ptr != nullptr){
        delete int_ptr;
    }
}