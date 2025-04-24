#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int i;
    pid_t pid;

    for (i = 0; i < 4; i++) {
        pid = fork();
        if (pid == 0) {
            // 자식 프로세스 작업
            printf("Child PID: %d\n", getpid());
            _exit(0);  // 자식 즉시 종료
        } else if (pid > 0) {
            wait(NULL);  // 자식 종료 대기
        }
    }
    return 0;
}
