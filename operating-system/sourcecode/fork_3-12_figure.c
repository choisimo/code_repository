#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 5

int num[SIZE] = {0,1,2,3,4};

int main() {
    int i;
    pid_t pid;

    pid = fork();

    if (pid == 0) {
        for (i = 0; i < SIZE; i++) {
            num[i] = -i;
            printf("CHILD: %d ", num[i]);
        }
    } else if (pid > 0) {
        wait(NULL);
        for(i = 0; i < SIZE; i++) {
            printf("PARENT: %d ", num[i]);
        }
    }
    return 0;
}