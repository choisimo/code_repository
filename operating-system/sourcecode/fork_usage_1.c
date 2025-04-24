#include <stdio.h>
#include <unistd.h>

int main() {
    int i;
    pid_t pid, pid1;

    pid = fork();

    printf("PID is [%d]", pid);

    if(pid == 0) {
        pid1 = getpid();
        printf("PID is [%d]\n", pid);
        printf("PID1 is [%d]\n", pid1);
    }
    return 0;
}