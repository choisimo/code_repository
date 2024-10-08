#include <iostream>

using namespace std;

struct NODE {
    int nData;
    struct NODE* next;
};

int main()
{    
    NODE list[5] = {0};

    list[0].next = &list[1];
    list[1].next = &list[2];
    list[2].next = &list[3];
    list[3].next = &list[4];
    list[4].next = nullptr;
 
    list[0].nData = 1;
    list[1].nData = 2;
    list[2].nData = 3;
    list[3].nData = 4;
    list[4].nData = 5;

    for(int i = 0; i < 5; i++){
        printf("list[%d]: %d\n", i, list[i]);
    }

    NODE *ptmp = &list[0];
    while(ptmp != NULL){
        printf
    }
    return 0;
}