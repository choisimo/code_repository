#include <iostream>

using namespace std;

void fibonacci(int n, int* result) {
    if (n <= 1)
        *result = n;
    else {
        int a = 0;
        int b = 1;
        int fib = 0;
        for (int i = 2; i <= n; i++) {
            fib = a + b;
            a = b;
            b = fib;
        }
        *result = fib;
    }
}

int main() {
    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        int result;
        fibonacci(i, &result);
        cout << result << " ";
    }

    return 0;
}
