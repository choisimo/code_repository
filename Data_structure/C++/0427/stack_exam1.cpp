//
// Created by csimo on 4/27/2024.
//
#include <iostream>
#include "stack.h"

bool isValid(Stack<char> *stack1, const std::string& s)
{
    for (char c: s) {
        switch(c) {
            case '(': case '{' : case '[' :
                stack1->push(c);
                break;
            case ')' :
                if(stack1->isEmpty() || stack1->pop() != '(') return false;
                break;
            case '}' :
                if (stack1->isEmpty() || stack1->pop() != '{') return false;
                break;
            case ']' :
                if (stack1->isEmpty() || stack1->pop() != '[') return false;
                break;
        }
    }
    bool result = stack1->isEmpty();

    return result;
}
