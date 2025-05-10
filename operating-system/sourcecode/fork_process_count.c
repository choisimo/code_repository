#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
If a process executes the following code sequence on a Unix/Linux system,
how many total processes (including the original) will exist when the execution completes?
*/


int main() {
    fork();         // 2 processes
    if (fork()) {   // 2 more process for parent
        fork();
    }
    fork();
    return 0;
}


/**
처음에는 원본 프로세스 1개만 존재합니다.
fork();
첫 번째 fork()는 프로세스를 복제하여 총 2개의 프로세스가 됩니다.
if (fork()) {
두 프로세스 모두 이 라인을 실행
각각 새로운 자식을 생성하여 총 4개의 프로세스가 됨
부모 프로세스들(2개)은 fork()가 자식의 PID를 반환하므로 if문 안으로 진입
자식 프로세스들(2개)은 fork()가 0을 반환하므로 if문을 건너뜀
fork(); (if문 내부)
if문에 들어간 2개의 부모 프로세스가 각각 fork()를 호출
추가적으로 2개의 프로세스가 생성되어 총 6개의 프로세스가 됨
fork(); (모든 프로세스가 실행)
6개의 모든 프로세스가 이 마지막 fork()를 실행
각각 새로운 자식을 생성하여 6개의 프로세스가 추가됨
최종적으로 12개의 프로세스가 존재
*/