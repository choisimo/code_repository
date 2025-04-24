# 첨부 파일과 코드 관련 문제 해결

## 문제 1 (Figure 3.21)
```c
#include 
#include 
int main()
{
int i;
for (i = 0; i  2)
- i=1 반복 후: 4개 프로세스 (2 -> 4)
- i=2 반복 후: 8개 프로세스 (4 -> 8)
- i=3 반복 후: 16개 프로세스 (8 -> 16)

fork()가 호출될 때마다 프로세스 수는 2배씩 증가하므로, 최종 프로세스 수는 2^4 = 16개입니다.

# 프로세스 생성 및 IPC(Inter-Process Communication) 이해하기

제공된 코드는 Unix/Linux 환경에서 프로세스 생성 및 제어를 보여주는 기본적인 예제입니다. 코드의 각 부분을 자세히 살펴보고 관련 개념을 정리해 보겠습니다.

## 코드 분석

```c
#include 
#include 
#include 
int main()
{
    pid_t pid;
    /* fork a child process */
    pid = fork();
    if (pid  0`): `wait()`를 호출하여 자식 프로세스 종료 대기
5. 자식 프로세스 종료: 부모 프로세스의 `wait()` 종료
6. 부모 프로세스: "Child Complete" 출력 후 종료

## 프로세스 생성 및 PCB 관리

fork() 시스템 콜을 호출하면 커널은 다음과 같이 PCB(Process Control Block)를 관리합니다:

### 복제되는 PCB 값들

1. **메모리 이미지**: 자식은 부모의 데이터, 힙, 스택을 동일하게 복사
2. **열린 파일 디스크립터**: 부모의 모든 파일 디스크립터가 자식에게 복사됨
3. **레지스터 값**: 프로그램 카운터(PC)를 포함한 CPU 레지스터
4. **신호 핸들러**: 부모의 신호 처리 설정

### 새롭게 할당되는 PCB 값들

1. **프로세스 ID (PID)**: 자식에게 새로운 고유 PID 할당
2. **PPID(Parent Process ID)**: 자식의 PPID는 부모의 PID가 됨
3. **프로세스 실행 시간**: 자식은 0부터 시작
4. **리소스 사용 통계**: CPU 시간, 메모리 사용량 등은 새로 시작

### 메모리 관리: 복사-쓰기(Copy-on-Write)

최신 OS에서는 효율성을 위해 fork() 호출 시 실제 메모리 복사를 즉시 수행하지 않습니다:

1. 처음에는 부모 프로세스의 페이지 테이블을 공유하며 모든 페이지는 읽기 전용으로 표시
2. 어느 한 프로세스가 메모리를 수정하려고 할 때 페이지 폴트 발생
3. 해당 페이지만 복사하여 수정 작업 진행
4. 이러한 방식으로 실제 필요한 페이지만 복사되어 메모리 효율성 증가

## IPC(프로세스 간 통신) 모델

IPC는 프로세스 간에 데이터를 교환하는 메커니즘을 제공합니다. 두 가지 주요 모델이 있습니다:

### 공유 메모리 모델

```
Process A  [Shared Memory Region]  Process B
```

특징:
- 통신하는 프로세스들이 공통 메모리 영역을 설정하고 공유
- 설정 후에는 일반적인 메모리 접근처럼 빠른 통신 가능
- 동기화 문제는 프로세스 자체에서 해결해야 함

장점:
- 커널 개입 없이 빠른 데이터 교환 가능
- 대용량 데이터 교환에 효율적

단점:
- 동기화 메커니즘 필요
- 분산 시스템에서 구현이 어려움

### 메시지 패싱 모델

```
Process A --send()--> [OS Kernel] --receive()--> Process B
```

특징:
- 프로세스 간에 메시지를 송수신
- 커널이 메시지 전달을 관리

장점:
- 구현이 비교적 간단
- 분산 시스템에 적합
- 충돌 회피 로직이 필요 없음

단점:
- 시스템 호출로 인한 오버헤드
- 대용량 데이터 전송에 비효율적

## 예제 문제 및 해설

### 문제 1: fork() 결과값 추적

다음 코드의 출력을 예측하세요:

```c
#include 
#include 
int main() {
    pid_t pid;
    pid = fork();
    printf("Hello from pid=%d\n", pid);
    return 0;
}
```

**해설**:
이 코드는 두 줄의 출력을 생성합니다:
1. 부모 프로세스: `Hello from pid=자식PID값`
2. 자식 프로세스: `Hello from pid=0`

fork() 후에는 두 프로세스가 모두 printf() 문을 실행하지만, pid 값이 다릅니다. 출력 순서는 스케줄링에 따라 달라질 수 있습니다.

### 문제 2: 다중 fork() 추적

다음 코드에서 생성되는 총 프로세스 수는?

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

**해설**:
총 8개의 프로세스가 생성됩니다(2³).
- 첫 번째 fork()로 2개의 프로세스 생성
- 각 프로세스가 두 번째 fork()를 실행하여 총 4개로 증가
- 각 프로세스가 세 번째 fork()를 실행하여 총 8개로 증가
- 모든 프로세스가 "hello"를 출력하므로 총 8줄의 "hello"가 출력됩니다.

### 문제 3: 파일 디스크립터 공유

다음 코드의 출력을 예측하세요:

```c
#include 
#include 
#include 
int main() {
    int fd = open("test.txt", O_CREAT|O_WRONLY|O_TRUNC, 0644);
    write(fd, "Hello", 5);
    
    if (fork() == 0) {
        // 자식 프로세스
        write(fd, " Child", 6);
    } else {
        // 부모 프로세스
        write(fd, " Parent", 7);
    }
    close(fd);
    return 0;
}
```

**해설**:
test.txt 파일에는 "Hello Child Parent" 또는 "Hello Parent Child"가 기록됩니다.
- fork() 시 자식은 부모의 파일 디스크립터를 상속
- 파일 오프셋(현재 위치)도 공유되므로 순차적으로 기록
- 부모/자식의 실행 순서는 스케줄링에 따라 달라질 수 있음

### 문제 4: exec() 이해하기

다음 코드의 출력을 예측하세요:

```c
#include 
#include 
int main() {
    printf("Before fork\n");
    
    if (fork() == 0) {
        printf("Child before exec\n");
        execlp("echo", "echo", "From exec", NULL);
        printf("This won't print\n");
    } else {
        printf("Parent process\n");
    }
    
    printf("End of process\n");
    return 0;
}
```

**해설**:
가능한 출력 시퀀스:
```
Before fork
Child before exec
From exec
Parent process
End of process
End of process
```
또는 부모가 자식보다 먼저 실행될 수 있습니다. 중요한 점은:
- "This won't print"는 절대 출력되지 않음(exec()는 성공 시 반환하지 않음)
- "End of process"는 두 번 출력됨(부모와 자식 프로세스에서 각각)
- "From exec"는 echo 명령에 의해 출력됨

### 문제 5: wait() 이해하기

다음 코드의 출력을 예측하세요:

```c
#include 
#include 
#include 
int main() {
    pid_t pid;
    int status;
    
    pid = fork();
    if (pid == 0) {
        printf("Child running\n");
        sleep(2);
        printf("Child exiting\n");
        return 42;
    } else {
        printf("Parent waiting\n");
        wait(&status);
        printf("Child exit status: %d\n", WEXITSTATUS(status));
        printf("Parent exiting\n");
    }
    return 0;
}
```

**해설**:
출력:
```
Parent waiting
Child running
Child exiting
Child exit status: 42
Parent exiting
```
순서가 중요합니다:
- wait()는 자식 프로세스가 종료될 때까지 부모 프로세스를 블록
- WEXITSTATUS()는 자식의 종료 상태를 추출(0-255 범위)
- 자식은 return 42로 종료되므로 종료 상태는 42

## 고급 개념: 공유 메모리 vs 메시지 패싱 예제

### 생산자-소비자 문제 (공유 메모리)

```c
#define BUFFER_SIZE 10
typedef struct { /* 데이터 항목 */ } item;
item buffer[BUFFER_SIZE];
int in = 0, out = 0;

// 생산자 프로세스
void producer() {
    item next_produced;
    while (true) {
        /* next_produced에 항목 생성 */
        while (((in + 1) % BUFFER_SIZE) == out)
            ; /* 버퍼가 가득 차면 대기 */
        buffer[in] = next_produced;
        in = (in + 1) % BUFFER_SIZE;
    }
}

// 소비자 프로세스
void consumer() {
    item next_consumed;
    while (true) {
        while (in == out)
            ; /* 버퍼가 비어있으면 대기 */
        next_consumed = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        /* next_consumed 항목 사용 */
    }
}
```

이 예제는 공유 메모리를 통해 두 프로세스가 어떻게 통신하는지 보여줍니다. 순환 버퍼를 사용하여 데이터를 교환하며, 동기화는 busy-waiting 방식으로 처리합니다.

### 생산자-소비자 문제 (메시지 패싱)

```c
// 생산자 프로세스
void producer() {
    item next_produced;
    while (true) {
        /* next_produced에 항목 생성 */
        send(next_produced);
    }
}

// 소비자 프로세스
void consumer() {
    item next_consumed;
    while (true) {
        receive(next_consumed);
        /* next_consumed 항목 사용 */
    }
}
```

메시지 패싱 방식에서는 send()와 receive() 시스템 콜을 통해 데이터를 교환합니다. 이 방식에서는 OS 커널이 동기화와 버퍼링을 관리합니다.

## 주요 헷갈리는 개념 정리

1. **fork()와 exec()의 차이점**:
   - fork(): 현재 프로세스의 복제본 생성
   - exec(): 현재 프로세스의 메모리 이미지를 새 프로그램으로 대체

2. **wait()와 waitpid()의 차이점**:
   - wait(): 아무 자식 프로세스의 종료를 기다림
   - waitpid(): 특정 자식 프로세스의 종료를 기다릴 수 있음

3. **fork() 후 메모리 독립성**:
   - 자식과 부모는 별도의 메모리 공간을 가짐
   - 복사-쓰기(Copy-on-Write) 기법으로 효율성 증가
   - 한 프로세스에서 변수 변경은 다른 프로세스에 영향 없음

4. **파일 디스크립터 상속**:
   - 파일 디스크립터는 복제됨
   - 파일 오프셋은 공유됨
   - 따라서 fork() 후 한 프로세스의 파일 읽기/쓰기는 다른 프로세스에 영향을 줌

5. **exec()의 반환값**:
   - 성공 시 절대 반환하지 않음
   - 실패 시에만 -1 반환
   - exec() 이후 코드는 실행되지 않음(성공 시)

이러한 개념들을 정확히 이해하면 Unix/Linux 시스템에서 프로세스 관리와 IPC를 효과적으로 다룰 수 있습니다.
