#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int i;
    for (i = 0; i < 3; i++) {
        if (fork() == 0) {
            printf("Child process %d\n", i);
        } else {
            printf("Parent process %d\n", i);
            wait(NULL);
        }
    }
    return 0;
}