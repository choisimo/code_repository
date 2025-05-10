//
// Created by nodove on 24. 12. 4.
//
#include <stdio.h>
#include <string.h>


int main(){
    char s[] = "abcdefgh";
    char copy_s[20] = "apple";
    printf("%s\n", copy_s);

    strcpy(copy_s, s);
    printf("%s\n", copy_s);
}