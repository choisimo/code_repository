# 운영체제에서의 프로세스와 스레드 이해하기

## 3장: 프로세스의 이해

프로세스는 운영체제의 가장 기본적인 추상화 개념 중 하나로, 실행 중인 프로그램의 인스턴스를 의미합니다. 

### 프로세스 생성과 종료

**Unix/Linux 시스템**에서 프로세스 생성은 `fork()` 시스템 콜을 통해 이루어집니다:

```c
pid_t pid = fork();
if (pid 
#include 
int main() {
    int i;
    for (i = 0; i 

void *runner(void *param) {
    // 스레드가 실행할 코드
    pthread_exit(0);
}

int main() {
    pthread_t tid;
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, runner, argv[1]);
    pthread_join(tid, NULL);
    
    return 0;
}
```

#### Windows 스레드

```c
DWORD WINAPI ThreadFunc(LPVOID param) {
    // 스레드가 실행할 코드
    return 0;
}

int main() {
    HANDLE threadHandle;
    DWORD threadId;
    
    threadHandle = CreateThread(NULL, 0, ThreadFunc, &param, 0, &threadId);
    WaitForSingleObject(threadHandle, INFINITE);
    CloseHandle(threadHandle);
    
    return 0;
}
```

#### Java 스레드

**Runnable 인터페이스 구현:**
```java
class Task implements Runnable {
    public void run() {
        // 스레드가 실행할 코드
    }
}

// 스레드 생성 및 시작
Thread thread = new Thread(new Task());
thread.start();

// 스레드 종료 대기
thread.join();
```

**Java Executor 프레임워크:**
```java
class Task implements Callable {
    public Integer call() {
        // 결과를 반환하는 스레드 코드
        return result;
    }
}

ExecutorService executor = Executors.newSingleThreadExecutor();
Future result = executor.submit(new Task());
Integer value = result.get(); // 결과 얻기
```

## 헷갈리기 쉬운 문제들

### 프로세스 관련 문제

**문제 1: 다음 코드를 실행하면 "LINE J"가 출력될까요?**
```c
pid_t pid = fork();
if (pid  0) { /* 부모 프로세스 */
    wait(NULL);
    for (int i = 0; i  {
        for (int j = 0; j 
#include 

void *thread_func(void *arg) {
    pthread_detach(pthread_self());
    printf("Thread running\n");
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    pthread_join(tid, NULL);
    printf("Thread joined\n");
    return 0;
}
```

**답변**: 이 코드는 교착 상태(deadlock)에 빠질 가능성이 있습니다. 스레드가 자신을 분리(detach)하면, 메인 스레드에서 `pthread_join()`을 호출할 때 오류가 발생하거나 영원히 블록될 수 있습니다. 분리된 스레드는 `join`할 수 없기 때문입니다. 이러한 코드는 race condition을 만듭니다 - 메인 스레드가 `pthread_join()`을 호출하기 전에 스레드가 분리되면 문제가 발생합니다.