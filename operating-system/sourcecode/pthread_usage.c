#include <stdio.h>
#include <pthread.h>

int value = 0;
void *runner(void *param); 

int main(int argc, char* argv[])
{
    pid_t pid;
    pthread_t tid;
    pthread_attr_t attr;

    pid = fork();

    if (pid == 0) {
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, runner, NULL);
        // thread 작업 완료 대기 
        //pthread_join(tid,NULL);
        printf("PTHREAD: tid = %ld", tid);
        if (tid) {
            printf("THREAD: value = %d", value);
        }
        printf("CHILD: value = %d", value);
    }
    else if (pid > 0) {
        wait(NULL);
        printf("PARENT: value = %d", value);
    }
}


void *runner(void* param) {
    value = 5;
    pthread_exit(0);
}