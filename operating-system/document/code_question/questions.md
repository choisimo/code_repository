# 운영체제 fork() 시스템 콜 문제

## 문제 1
다음 C 코드의 출력을 예측하시오. 왜 이런 결과가 나오는지 설명하시오.

```c
#include 
#include 

int main() {
    printf("Hello ");
    fork();
    printf("World\n");
    return 0;
}
```

**답변:**
출력: `Hello World` `World`

**해설:**
이 프로그램은 "Hello World" 두 번이 아닌 "Hello World"와 "World"를 출력합니다. 이는 stdout이 버퍼링되기 때문입니다. `printf("Hello ")`가 실행될 때 "Hello "는 출력 버퍼에 저장되지만 줄바꿈 문자가 없어 즉시 화면에 표시되지 않습니다. 그 후 `fork()`가 호출되면 자식 프로세스가 생성되고, 부모 프로세스의 모든 메모리 상태(출력 버퍼 포함)를 복사합니다. 따라서 부모와 자식 프로세스 모두 버퍼에 "Hello "를 가지고 있습니다. 두 프로세스가 각각 `printf("World\n")`를 실행하면, 줄바꿈 문자로 인해 버퍼가 플러시되어 "Hello World"가 출력되고, 두 번째 실행에서는 "World"만 출력됩니다[3][6].

## 문제 2
다음 코드가 생성하는 총 프로세스 수를 계산하고, 결과를 설명하시오.

```c
#include 
#include 

int main() {
    fork();
    fork();
    fork();
    printf("hello\n");
    return 0;
}
```

**답변:**
총 프로세스 수: 8개

**해설:**
이 코드는 총 8개의 프로세스를 생성합니다. 처음에는 하나의 프로세스만 있습니다. 첫 번째 `fork()`는 이를 2개로 만듭니다. 그 후 각 프로세스가 두 번째 `fork()`를 실행하여 총 4개가 됩니다. 마지막으로, 모든 4개 프로세스가 세 번째 `fork()`를 실행하여 총 8개의 프로세스가 됩니다. 일반적으로, n개의 `fork()` 호출은 2^n개의 프로세스를 생성합니다. 따라서 3개의 `fork()` 호출은 2^3 = 8개의 프로세스를 생성합니다. 각 프로세스는 "hello"를 출력하므로, "hello"가 8번 출력됩니다[2][5].

## 문제 3
다음 코드의 출력을 예측하고, 프로세스 생성 과정을 설명하시오.

```c
#include 
#include 

int main() {
    int x = 1;
    
    if(fork() == 0) {
        printf("Child: x=%d\n", ++x);
        exit(0);
    }
    
    printf("Parent: x=%d\n", --x);
    return 0;
}
```

**답변:**
출력:
```
Parent: x=0
Child: x=2
```
또는
```
Child: x=2
Parent: x=0
```

**해설:**
이 코드에서 `fork()`는 새로운 자식 프로세스를 생성합니다. `fork()`의 반환값은 부모 프로세스에서는 자식 프로세스의 PID(양수), 자식 프로세스에서는 0입니다. 따라서 `if(fork() == 0)`은 자식 프로세스에서만 참이 됩니다.

자식 프로세스에서는 x를 1 증가시켜 2로 만들고 출력한 후 종료합니다. 부모 프로세스에서는 x를 1 감소시켜 0으로 만들고 출력합니다. 중요한 것은 `fork()` 후에 각 프로세스가 독립적인 메모리 공간을 가지므로, 한 프로세스에서 변수를 변경해도 다른 프로세스에는 영향을 미치지 않습니다[5].

출력 순서는 프로세스 스케줄링에 따라 달라질 수 있으므로, 두 가지 출력 순서 모두 가능합니다.

## 문제 4
다음 코드의 가능한 모든 출력을 나열하고, 각 출력이 나올 수 있는 이유를 설명하시오.

```c
#include 
#include 

int main() {
    int i;
    for(i = 0; i 
#include 
#include 

int main() {
    pid_t pid = fork();
    
    if(pid 
#include 
#include 

int main() {
    pid_t pid = fork();
    
    if(pid 
#include 
#include 
#include 

int main() {
    int fd = open("test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd 
#include 
#include 

int main() {
    pid_t pid = fork();
    
    if(pid 
#include 
#include 
#include 

#define MAX_COUNT 10

int main() {
    pid_t pid;
    int i;
    int status;
    
    for(i = 1; i 
#include 
#include 
#include 

#define MAX_COUNT 10

int main() {
    pid_t pid;
    int i;
    int status;
    
    for(i = 1; i 
#include 
#include 

int main() {
    int x = 10;
    
    if(fork() && fork()) {
        x += 5;
        printf("A: x = %d\n", x);
    }
    
    if(fork() || fork()) {
        x += 10;
        printf("B: x = %d\n", x);
    }
    
    printf("C: x = %d\n", x);
    return 0;
}
```

**답변:**
이 코드는 복잡한 `fork()` 패턴과 논리 연산자를 사용하여 다양한 프로세스를 생성합니다.

**첫 번째 조건 `if(fork() && fork())`:**
- 첫 번째 `fork()`가 부모 프로세스에서는 양수(자식 PID)를 반환하므로 참입니다.
- 자식 프로세스에서는 0을 반환하므로 거짓이 되어, 두 번째 `fork()`는 실행되지 않습니다.
- 따라서 부모 프로세스만 두 번째 `fork()`를 실행하여 또 다른 자식을 생성합니다.
- 이 시점에서 총 3개의 프로세스가 존재합니다: 원본 부모, 첫 번째 자식, 두 번째 자식

**두 번째 조건 `if(fork() || fork())`:**
- 각 프로세스가 이 조건을 평가합니다.
- 첫 번째 `fork()`가 성공하면 부모에서는 참이 됩니다.
- 자식에서는 0을 반환하므로, 두 번째 `fork()`가 실행됩니다.

이 코드는 복잡한 분기를 통해 여러 프로세스를 생성하고, 각 프로세스는 자신의 x 값을 가지고 있으며, 조건에 따라 값을 증가시킵니다. 정확한 출력은 프로세스 생성 순서와 스케줄링에 따라 달라질 수 있습니다[2][5].

## 문제 10
다음 코드의 출력을 예측하고, 이 코드에서 발생할 수 있는 동기화 문제에 대해 설명하시오.

```c
#include 
#include 
#include 
#include 
#include 
#include 

int main() {
    int fd = open("counter.txt", O_RDWR | O_CREAT, 0644);
    if(fd < 0) {
        perror("open failed");
        return 1;
    }
    
    // 파일에 초기값 "0" 쓰기
    write(fd, "0", 1);
    lseek(fd, 0, SEEK_SET);
    
    if(fork() == 0) {
        // 자식 프로세스
        // 파일에서 카운터 읽기
        char buf[2];
        read(fd, buf, 1);
        buf[1] = '\0';
        
        // 카운터 증가
        int counter = atoi(buf) + 1;
        printf("Child: counter = %d\n", counter);
        
        // 다시 파일 위치를 처음으로
        lseek(fd, 0, SEEK_SET);
        
        // 증가된 값 쓰기
        sprintf(buf, "%d", counter);
        write(fd, buf, 1);
        
        close(fd);
        return 0;
    }
    
    // 부모 프로세스
    // 잠시 대기 (자식이 먼저 실행되도록)
    sleep(1);
    
    // 파일에서 카운터 읽기
    char buf[2];
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, 1);
    buf[1] = '\0';
    
    // 카운터 증가
    int counter = atoi(buf) + 1;
    printf("Parent: counter = %d\n", counter);
    
    // 다시 파일 위치를 처음으로
    lseek(fd, 0, SEEK_SET);
    
    // 증가된 값 쓰기
    sprintf(buf, "%d", counter);
    write(fd, buf, 1);
    
    // 자식 프로세스 대기
    wait(NULL);
    
    close(fd);
    return 0;
}
```

**답변:**
예상 출력:
```
Child: counter = 1
Parent: counter = 1
```

파일 counter.txt의 최종 내용: "1"

**해설:**
이 코드는 부모와 자식 프로세스가 모두 같은 파일에서 카운터 값을 읽고, 1을 증가시킨 후 다시 파일에 쓰는 작업을 수행합니다. 그러나 이 코드에는 동기화 문제가 있습니다:

1. 경쟁 조건(Race Condition):
   - 자식과 부모 프로세스가 동시에 파일에 접근하여 각자 카운터 값을 읽고 1 증가시킨 후 다시 쓸 수 있습니다.
   - 이 경우, 두 프로세스 모두 원래 값("0")을 읽고 1을 증가시켜 "1"을 쓰게 됩니다.
   - 따라서 두 프로세스가 모두 카운터를 증가시켰음에도 불구하고, 최종 값은 "1"이 됩니다. 올바른 결과는 "2"여야 합니다.

2. 동기화 해결책:
   - 파일 락(file locking)을 사용하여 한 번에 하나의 프로세스만 파일에 접근할 수 있도록 합니다.
   - 세마포어나 다른 IPC 메커니즘을 사용하여 프로세스 간 동기화를 구현합니다.
   - 하나의 트랜잭션으로 읽기-수정-쓰기 작업을 보장합니다[3][6].

이 코드에서는 `sleep(1)`을 통해 부모 프로세스가 자식 프로세스 후에 실행되도록 시도하고 있지만, 이는 완벽한 동기화를 보장하지 않습니다. 실제 시스템에서는 스케줄링에 따라 결과가 달라질 수 있습니다.
