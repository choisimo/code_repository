#include <iostream>
#include <string>

using namespace std;

int main(){
    
    int a[] = {10, 20, 30, 40, 50};
    cout << a << endl;
    cout << a + 1 << endl;
    cout << *a << endl;
    cout << *a + 1 << endl;
    cout << *(a+1) << endl;
    cout << &a << endl;
    cout << *(&a) << endl;
    cout << &(*(&a)) << endl;
    *a = 5;
    cout << &a << endl;

    return 0;    
}