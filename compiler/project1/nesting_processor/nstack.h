#ifndef NSTACK_H
#define NSTACK_H

#include <stdbool.h>

typedef struct nstack {
    char* arr;
    int capacity;
    int top;
} nstack;

// create stack
nstack* nstack_creat(int capacity);

// delete stack
void nstack_delete(nstack* stack);

// check if empty
bool empty(nstack* stack);

// check if full
bool full(nstack* stack);

// add element to the stack
bool push(nstack* stack, char item);

// delete element from the stack 
char pop(nstack* stack);

// peak the stack
char peak(nstack* stack);

#endif