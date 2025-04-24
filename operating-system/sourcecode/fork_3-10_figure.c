#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    
    pid_t pid, pid1; 

    pid = fork(); // child 는 fork() 와 함께 생김

    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    }
    else if (pid == 0) {
        pid1 = getpid();
        printf("child: pid = %d", pid); // assume.. 0
        printf("child: pid1 = %d", pid1); // assume.. 자식 PID
    }
    else {
        pid1 = getpid();
        printf("parent: pid = %d", pid);   //  assume.. 자식 PID
        printf("parent: pid1 = %d", pid1); // assume.. 부모 PID
        wait(NULL);
    }
    return 0;
}


// 커널은 부모 프로세스의 거의 완벽한 복제본을 생성합니다
// 자식 프로세스는 다음을 상속받습니다:
// 메모리 이미지 (텍스트, 데이터, 스택, 힙 세그먼트)
// 파일 디스크립터 테이블
// 현재 작업 디렉토리
// 환경 변수 등
// 한 가지 중요한 차이: fork() 반환값
// 부모 프로세스: 자식의 PID (양수)
// 자식 프로세스: 0
// 오류 발생 시: -1

