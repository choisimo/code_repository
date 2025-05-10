//
// Created by csimo on 9/26/2024.
//

#include "Adder.h"

Adder::Adder(int a, int b) {
    this->op1 = a; this-> op2 = b;
}

int Adder::process() {
    return this->op1 + this->op2;
}