#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int status1, status2, status3;
    pid_t pid1, pid2, pid3;

    if (argc < 2) {
        fprintf(stderr, "사용법: %s <파일 이름>\n", argv[0]);
        exit(1);
    }

    pid1 = fork();
    if (pid1 == 0) {
        printf("[%d]\n", getpid());
        execl("./ac_create", "ac_create", argv[1], (char *)NULL);
        exit(1);
    }
    waitpid(pid1, &status1, 0);

    pid2 = fork();
    if (pid2 == 0) {
        printf("[%d]\n", getpid());
        execl("./ac_update", "ac_update", argv[1], (char *)NULL);
        exit(1);
    }
    waitpid(pid2, &status2, 0);

    pid3 = fork();
    if (pid3 == 0) {
        printf("[%d]\n", getpid());
        execl("./ac_query", "ac_query", argv[1], (char *)NULL);
        exit(1);
    }
    waitpid(pid3, &status3, 0);

    return 0;
}
