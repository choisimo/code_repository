#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int cnt = 0;
int *count = &cnt;

int main() {
    pid_t pid, pid2, pid3;

    pid = fork();
    pid2 = fork();
    pid3 = fork();

    printf("%p", count);
    printf(" | ");
    printf("%p", &count);
    printf(" | ");
    printf("%d", *count);

    if ((pid||pid2||pid3) == 0) {
        printf("CHILD: call count[%d]\n", *(count)++);
        return 0;
    }
    else if ((pid||pid2||pid3) > 0) {
        wait(NULL);
        printf("PARENT: call count[%d]\n", *(count)++);
        return 0;
    }
    else {
        free(count);
    }
}