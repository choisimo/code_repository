//
// Created by nodove on 24. 11. 26.
//
#include <iostream>
using namespace std;

class Adder {
protected:
    int add(int a, int b) {
        return a + b;
    }
};

class Subtractor {
protected:
    int sub(int a, int b) {
        return a - b;
    }
};

class Multiplier {
protected:
    int mul(int a, int b) {
        return a * b;
    }
};

class Divider {
protected:
    int div(int a, int b) {
        return a / b;
    }
};

class Calculator : public Adder, public Subtractor, public Multiplier, public Divider {
public:
    int calc(char op, int a, int b);
};

int Calculator::calc(char op, int a, int b){
    int result = 0;
    switch(op)
    {
        case '+':
            result = add(a, b);
            break;
        case '-':
            result = sub(a, b);
            break;
        case '*':
            result = mul(a, b);
            break;
        case '/':
            result = div(a, b);
            break;
    }
    return result;
}

int main() {
    Calculator calc;
    cout << "3 + 2 = " << calc.calc('+', 3, 2) << endl;
    cout << "3 - 2 = " << calc.calc('-', 3, 2) << endl;
    cout << "3 * 2 = " << calc.calc('*', 3, 2) << endl;
    cout << "3 / 2 = " << calc.calc('/', 3, 2) << endl;
    return 0;
}