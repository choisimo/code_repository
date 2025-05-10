#include "nstack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// create stack
nstack* nstack_creat(int capacity) {
    nstack* stack = (nstack *)malloc(sizeof(nstack));
    
    if (stack == NULL) {
        printf("failed to allocate memory");
        return NULL;
    }

    stack->capacity = capacity;
    stack->top = -1;
    stack->arr = (char *)malloc(capacity * sizeof(char));

    if (stack->arr == NULL){
        free(stack);
        return NULL;
    }

    return stack;
}

// delete stack
void nstack_delete(nstack* stack) {
    if (stack != NULL) {
        free(stack->arr);
        free(stack);
    }
}

// check if empty
bool empty(nstack* stack) {
    return stack->top == -1;
}

// check if full
bool full(nstack* stack) {
    return stack->top == stack->capacity -1;
}

// add element to the stack
bool push(nstack* stack, char item) {
    if (full(stack)) {
        int new_capacity = stack->capacity * 2;
        char* newArr = (char *)malloc(new_capacity * sizeof(char));

        if (newArr == NULL) {
            printf("failed to allocate memory\n");
            return false;
        }

        memcpy(newArr, stack->arr, stack->capacity * sizeof(char));
        free(stack->arr);
        stack->arr = newArr;
        stack->capacity = new_capacity;
    }
    stack->arr[++stack->top] = item;
    return true;
}

// delete element from the stack 
char pop(nstack* stack) {
    if (empty(stack)) {
        printf("stack underflow\n");
        exit(1);
    }
    return stack->arr[stack->top--];
}

// peak the stack
char peak(nstack* stack) {
    if (empty(stack)) {
        printf("stack underflow\n");
        exit(1);
    }
    return stack->arr[stack->top];
}