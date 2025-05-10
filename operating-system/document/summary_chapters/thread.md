# 스레딩 개념: 어려운 문제와 헷갈리기 쉬운 개념들

## 스레드 라이브러리 구현 방법

1. **문제**: 사용자 수준 스레드와 커널 수준 스레드 구현의 차이점을 설명하고, 어떤 경우에 하나가 다른 것보다 선호되는지 시나리오를 식별하세요.
   **해답**: 사용자 수준 스레드는 커널 지원 없이 스레드 라이브러리에 의해 관리되어 더 빠른 스레드 연산(생성, 전환)을 허용하지만 CPU 간의 진정한 병렬성을 방해합니다. 커널 수준 스레드는 OS 커널에 의해 관리되어 병렬 실행을 허용하지만 스레드 연산에 더 높은 오버헤드가 있습니다. 컨텍스트 전환 속도가 중요한 짧은 수명의 스레드가 많은 애플리케이션에는 사용자 수준 스레드가 선호되며, 다중 CPU 코어에서 병렬 실행의 이점을 얻을 수 있는 애플리케이션에는 커널 수준 스레드가 선호됩니다.

2. **문제**: 사용자 수준 스레드 구현에서 한 스레드가
 블로킹 시스템 호출을 할 때 어떤 문제가 발생하며, 이를 어떻게 해결할 수 있을까요?
   **해답**: 사용자 수준 구현에서 스레드가 블로킹 시스템 호출을 하면 전체 프로세스가 블록되어 다른 모든 스레드도 블록됩니다. 이 문제는 여러 방법으로 해결할 수 있습니다: (1) select()를 사용하여 연산이 블록될지 확인하는 래퍼 함수와 함께 비블로킹 시스템 호출 사용, (2) 신호나 콜백이 있는 비동기 시스템 호출 사용, (3) 시스템 호출을 가로채서 특별하게 처리하는 "재킷팅(jacketing)" 접근 방식 사용.

3. **문제**: "many-to-many" 스레딩 모델을 설명하고 "one-to-one" 및 "many-to-one" 모델에 비해 장점을 설명하세요.
   **해답**: many-to-many 모델은 많은 사용자 수준 스레드를 더 적거나 같은 수의 커널 스레드에 매핑합니다. 장점으로는: (1) 진정한 병렬성을 허용함으로써 many-to-one 모델의 한계를 극복함, (2) 각 사용자 스레드가 커널 스레드를 필요로 하는 one-to-one 모델의 확장성 문제가 없음, (3) 스레드 라이브러리가 사용 가능한 커널 스레드에 사용자 스레드를 효율적으로 스케줄링할 수 있어 두 모델의 이점을 결합함.

4. **문제**: 스레드 라이브러리를 구현할 때 스레드 생성 중에 어떤 경쟁 조건이 발생할 수 있으며, 일반적으로 어떻게 방지합니까?
   **해답**: 자식 스레드가 완전히 초기화되거나 스케줄되기 전에 부모 스레드가 실행을 완료하면 경쟁 조건이 발생할 수 있습니다. 이로 인해 자식 스레드가 실행되기 전에 프로세스가 종료될 수 있습니다. 이는 일반적으로 다음과 같이 방지됩니다: (1) 부모가 자식 초기화를 기다리도록 뮤텍스나 세마포어와 같은 명시적 동기화 메커니즘 사용, (2) 부모에게 제어를 반환하기 전에 자식 스레드가 제대로 초기화되도록 스레드 라이브러리가 보장, (3) 부모가 자식 완료를 기다리도록 스레드 조인 메커니즘 사용.

5. **문제**: 다양한 스레드 구현 모델에서 주소 공간이 어떻게 관리되며 이것이 스레드 안전성에 어떤 영향을 미치는지 설명하세요.
   **해답**: 모든 스레딩 모델에서 스레드는 프로세스 내에서 동일한 주소 공간을 공유합니다. 그러나 각 스레드는 로컬 변수를 위한 자체 스택을 가집니다. 이는 다음을 의미합니다: (1) 전역 변수는 모든 스레드가 접근 가능하므로 경쟁 조건 방지를 위한 동기화가 필요함, (2) 각 스레드는 자체 스택에 비공개 로컬 변수를 가지며, 이는 동시 접근으로부터 안전함, (3) 스레드별 데이터는 스레드 라이브러리가 제공하는 스레드 로컬 스토리지 메커니즘을 통해 명시적으로 관리되어야 함. 스레드 안전성은 어떤 데이터가 공유되는지 대 스레드 비공개인지를 신중하게 고려해야 합니다.

## POSIX Pthreads

1. **문제**: `pthread_mutex_lock()`과 `pthread_mutex_trylock()`의 차이점은 무엇이며, 어떤 상황에서 하나를 다른 것보다 사용하겠습니까?
   **해답**: `pthread_mutex_lock()`은 뮤텍스가 획득될 때까지 무기한 블록되는 반면, `pthread_mutex_trylock()`은 뮤텍스가 이미 잠겨있는 경우 즉시 오류 코드와 함께 반환됩니다. `pthread_mutex_trylock()`은 스레드가 뮤텍스를 사용할 수 없을 때 수행할 수 있는 대안 작업이 있을 때(교착 상태 방지), 타임아웃 구현, 또는 비블로킹 알고리즘 구현에 유용합니다. `pthread_mutex_lock()`은 스레드가 진행하기 전에 리소스에 대한 독점적 접근을 위해 기다려야 할 때 적절합니다.

2. **문제**: 취소 가능성 유형이 `PTHREAD_CANCEL_DEFERRED`로 설정된 경우와 `PTHREAD_CANCEL_ASYNCHRONOUS`로 설정된 경우의 `pthread_cancel()` 차이점을 설명하세요. 왜 비동기 취소가 거의 유용하지 않을까요?
   **해답**: `PTHREAD_CANCEL_DEFERRED`에서는 취소가 취소 포인트(sleep()이나 pthread_testcancel()과 같은 특정 함수)에서만 발생합니다. `PTHREAD_CANCEL_ASYNCHRONOUS`에서는 취소가 어떤 명령어에서든 발생할 수 있습니다. 비동기 취소는 다음과 같은 이유로 거의 유용하지 않습니다: (1) 메모리 할당과 같은 중요한 작업 중에 스레드가 취소될 수 있어 안전하지 않음, (2) 스레드가 자원을 안정적으로 해제하거나 공유 상태를 복원할 수 없음, (3) 뮤텍스를 보유한 채 취소되면 교착 상태로 이어질 수 있음. 지연된 취소는 스레드가 안전하게 취소될 수 있는 제어된 지점을 제공합니다.

3. **문제**: Pthreads에서 클린업 핸들러의 목적을 설명하고 이들이 필수적인 시나리오의 예를 제공하세요.
   **해답**: 클린업 핸들러는 `pthread_cleanup_push()`로 등록된 함수로, 스레드가 취소되거나 `pthread_exit()`를 호출할 때 자동으로 호출됩니다. 이들은 적절한 자원 정리와 상태 복원을 허용합니다. 다음과 같은 경우에 필수적입니다: (1) 스레드가 해제해야 할 메모리를 할당한 경우, (2) 스레드가 해제해야 할 뮤텍스를 잠근 경우, (3) 파일 디스크립터나 다른 자원을 닫아야 하는 경우, (4) 공유 데이터 구조를 일관된 상태로 복원해야 하는 경우. 예: 데이터베이스 작업을 처리하는 스레드는 취소될 경우 데이터베이스 락을 해제하고 불완전한 트랜잭션을 롤백하기 위한 클린업 핸들러가 필요할 수 있습니다.

4. **문제**: 멀티스레드 프로그램에서 `fork()`를 호출하는 의미는 무엇이며, `pthread_atfork()` 함수는 이러한 문제를 해결하는 데 어떻게 도움이 될 수 있을까요?
   **해답**: 멀티스레드 프로세스에서 `fork()`가 호출되면 호출 스레드만 자식 프로세스에 복제되지만, 모든 뮤텍스와 조건 변수는 현재 상태로 복사됩니다. 이로 인해: (1) 자식에 잠긴 뮤텍스가 있지만 잠금을 해제할 스레드가 없음, (2) 공유 데이터 구조의 일관성 없는 상태, (3) 스레드별 데이터 소멸자가 호출되지 않아 메모리 누수 발생. `pthread_atfork()` 함수는 fork 전후에 호출될 핸들러를 등록하여: fork 전에 모든 뮤텍스를 잠그고, fork 후 부모에서 부모 상태를 복원하고, fork 후 자식에서 자식 상태(뮤텍스 잠금 해제)를 재설정합니다.

5. **문제**: Pthreads 프로그램에서 신호가 스레드와 어떻게 상호 작용하며, 멀티스레드 애플리케이션에서 신호를 안전하게 처리하기 위한 전략은 무엇입니까?
   **해답**: Pthreads 프로그램에서: (1) 신호 처리(핸들러)는 프로세스 전체적이며 모든 스레드가 공유함, (2) 신호 마스크는 스레드별로 다름, (3) 프로세스 대상 신호는 해당 신호를 차단하지 않는 어떤 스레드에게도 전달될 수 있음. 신호를 안전하게 처리하기 위한 권장 전략은: (a) 모든 스레드에서 모든 비동기 신호 차단, (b) `sigwait()`을 호출하여 신호를 동기적으로 수신하는 전용 스레드 생성, (c) 이 스레드가 신호에 대응하여 필요한 작업을 수행하고, 가능하면 조건 변수나 다른 동기화 메커니즘을 통해 다른 스레드와 통신. 이는 비동기 신호 안전 함수의 제한과 경쟁 조건을 피합니다.

## Windows 스레드

1. **문제**: Windows(`CreateThread()`)와 POSIX(`pthread_create()`)의 스레드 생성을 비교 대조하세요. 어떤 매개변수가 다르며 Windows API는 어떤 추가 기능을 제공합니까?
   **해답**: Windows `CreateThread()`는 `pthread_create()`와 여러 가지 다른 점이 있습니다: (1) 포인터 매개변수에 ID를 저장하는 대신 스레드 핸들을 반환함, (2) 보안 속성과 초기 스택 크기를 매개변수로 지정할 수 있음, (3) 스레드가 일시 중단 상태로 시작하도록 지정할 수 있는 생성 플래그 매개변수가 있음, (4) `CloseHandle()`로 닫아야 하는 핸들을 반환함(더 단순한 Pthreads 접근 방식과 달리). Windows는 스레드 일시 중단/재개와 생성 시 직접적인 스레드 우선순위 제어와 같은 추가 기능을 제공합니다.

2. **문제**: Windows의 `SuspendThread()`를 사용하여 스레드를 일시 중단하는 것과 스레드가 Event와 같은 동기화 객체를 기다리게 하는 것의 차이점을 설명하세요. `SuspendThread()`를 사용할 때의 위험은 무엇입니까?
   **해답**: `SuspendThread()`는 스레드가 무엇을 하고 있는지에 상관없이 스레드 실행을 강제로 일시 중단하는 반면, Event를 기다리는 것은 스레드가 자발적으로 기다리는 협력적 메커니즘입니다. `SuspendThread()`의 위험에는: (1) 스레드가 락을 보유하거나 공유 데이터 구조를 업데이트하는 중에 일시 중단될 수 있음, (2) 커널 코드 실행 중이나 내부 락을 보유한 채 일시 중단되면 교착 상태를 유발할 수 있음, (3) 스레드 간 동기화에 안전하게 사용할 수 없음. Event는 스레드가 잘 정의된 지점에서 기다리고 기다리기 전에 자원을 적절히 해제하기 때문에 더 안전합니다.

3. **문제**: Windows 스레딩에서 스레드 로컬 스토리지(TLS)의 중요성과 POSIX 등가물과 어떻게 비교되는지 설명하세요.
   **해답**: Windows TLS는 각 스레드가 TLS 인덱스를 통해 접근할 수 있는 자체 변수 복사본을 가질 수 있게 합니다. 이는 트랜잭션 컨텍스트나 오류 상태와 같은 스레드별 데이터에 유용합니다. POSIX의 스레드별 데이터(`pthread_key_create()`, `pthread_setspecific()` 등으로 구현)와 비교하여, Windows TLS는: (1) 인덱스 수가 제한됨(최신 Windows에서 1088개), (2) `TlsAlloc()`/`TlsFree()`로 더 간단한 할당/해제, (3) 스레드가 종료될 때 호출되는 소멸자를 기본적으로 제공하지 않음(POSIX와 달리), 수동 정리 필요, (4) API 기반과 컴파일러 지원(`__declspec(thread)`) 구현 모두 제공.

4. **문제**: Windows가 기본 뮤텍스 외에 제공하는 스레드 동기화 메커니즘은 무엇이며, 어떤 시나리오에서 각각을 사용하겠습니까?
   **해답**: 기본 뮤텍스 외에도 Windows는 다음을 제공합니다: (1) 크리티컬 섹션: 오버헤드가 적은 프로세스 전용 뮤텍스로, 프로세스 내에서 빈번하고 짧은 동기화에 가장 적합, (2) 이벤트: 조건이 발생했을 때 스레드 간 신호를 보내는 데 사용, 수동 재설정 및 자동 재설정 변형으로 제공, (3) 세마포어: 제한된 자원 풀에 대한 접근 제어, (4) 조건 변수: 스레드가 크리티컬 섹션을 해제하면서 특정 조건을 기다릴 수 있음, (5) Slim Reader/Writer Locks: 많은 리더와 적은 라이터가 있는 시나리오에 최적화, (6) Interlocked Functions: 전체 락 오버헤드 없이 간단한 원자적 연산 수행.

5. **문제**: Windows가 스레드 우선순위를 어떻게 구현하며 프로세스 우선순위 클래스가 스레드 우선순위와 어떻게 상호 작용하는지 설명하세요. 스레드 우선순위를 조작할 때 어떤 문제가 발생할 수 있을까요?
   **해답**: Windows는 0-31 값으로 스레드 우선순위를 구현하며, 프로세스 우선순위 클래스와 스레드의 상대적 우선순위를 결합하여 결정됩니다. 스레드의 유효 우선순위는 다음과 같이 계산됩니다: (프로세스 우선순위 클래스 기반 + 스레드 상대적 우선순위). 발생할 수 있는 문제에는: (1) 우선순위 역전, 낮은 우선순위 스레드가 높은 우선순위 스레드가 필요로 하는 자원을 보유하는 경우, (2) 바쁜 시스템에서 낮은 우선순위 스레드의 기아 현상, (3) 동적 우선순위 부스팅으로 인한 다른 Windows 버전에서의 다른 동작, (4) 프로세스 우선순위 클래스를 고려하지 않고 다른 프로세스의 스레드 우선순위가 설정될 때 예상치 못한 성능, (5) 너무 많은 스레드가 높은 우선순위로 설정되면 시스템 응답성 문제.

## Java 스레드 구현

1. **문제**: Java에서 `Thread` 클래스를 확장하는 것과 `Runnable` 인터페이스를 구현하는 것을 비교하세요. 어떤 접근 방식이 더 유연하며 그 이유는 무엇입니까?
   **해답**: `Runnable`을 구현하는 것은 일반적으로 더 유연합니다. 그 이유는: (1) Java는 단일 상속만 지원하므로, `Thread`를 확장하면 클래스가 다른 클래스를 확장하지 못하게 됨, (2) `Runnable`은 수행되는 작업과 스레드 자체를 더 잘 분리하여 더 나은 설계를 촉진함, (3) 동일한 `Runnable`이 다른 스레드 메커니즘(스레드 풀, 실행자)에 의해 실행될 수 있음, (4) 여러 `Runnable` 객체가 동일한 스레드 자원을 공유할 수 있음, (5) `Runnable`은 람다 표현식과 같은 현대 Java 기능과 더 잘 작동함. `Thread`를 확장하는 것은 작은 애플리케이션에는 더 간단하지만 애플리케이션이 성장함에 따라 유연성이 제한됩니다.

2. **문제**: Java에서 `synchronized` 키워드의 중요성과 명시적 락(`java.util.concurrent.locks.Lock`)을 사용하는 것과의 차이점을 설명하세요.
   **해답**: `synchronized` 키워드는 객체에 대한 내재적 락을 자동으로 획득하여 한 번에 하나의 스레드만 코드 블록이나 메서드를 실행하도록 보장합니다. 명시적 `Lock` 객체와 비교하면: (1) `synchronized`는 더 단순하지만 덜 유연함, (2) `Lock`은 락 획득에 대한 비블로킹 시도(`tryLock()`)를 허용함, (3) `Lock`은 획득에 대한 시간 제한 대기를 지원함, (4) `Lock`은 획득된 것과 다른 메서드나 범위에서 해제될 수 있음, (5) `synchronized` 블록은 예외가 발생할 때 자동으로 락을 해제하여 더 안전함, (6) 명시적 `Lock` 객체는 `ReadWriteLock` 구현으로 읽기와 쓰기 접근을 구분할 수 있음.

3. **문제**: Java 스레드에서 `wait()`와 `sleep()`의 차이점은 무엇이며, 프로그래머들이 `wait()`와 관련하여 자주 범하는 실수는 무엇입니까?
   **해답**: 주요 차이점: (1) `wait()`는 호출된 객체의 모니터/락을 해제하는 반면 `sleep()`은 어떤 락도 해제하지 않음, (2) `wait()`는 Object에서 호출되는 반면 `sleep()`은 Thread 클래스에서 호출됨, (3) `wait()`는 동기화된 컨텍스트에서 호출되어야 함, (4) `wait()`는 `notify()`/`notifyAll()`에 의해 깨어날 수 있는 반면 `sleep()`은 항상 지정된 시간을 기다림(인터럽트 제외). 흔한 실수는 적절한 동기화 없이 또는 루프에서 조건을 확인하지 않고 `wait()`를 호출하는 것인데, 이는 신호를 놓치거나 거짓 깨움으로 이어질 수 있습니다. 올바른 패턴은: `synchronized(obj) { while(!condition) { obj.wait(); } }`.

4. **문제**: Java 스레드 구현에서 스레드에서 발생한 처리되지 않은 예외는 어떻게 되며, 이 동작을 어떻게 사용자 정의할 수 있습니까?
   **해답**: 기본적으로 스레드에서 처리되지 않은 예외는 JVM이: (1) 예외 스택 트레이스를 콘솔에 출력하고, (2) 스레드를 종료된 것으로 표시하고, (3) 다른 스레드에 예외를 전파하지 않고 계속 실행하게 합니다. 이 동작은 다음과 같이 사용자 정의할 수 있습니다: (a) `thread.setUncaughtExceptionHandler()`로 스레드별 `UncaughtExceptionHandler` 설정, (b) 스레드 그룹 예외 핸들러 설정, (c) `Thread.setDefaultUncaughtExceptionHandler()`를 사용하여 모든 스레드에 대한 기본 핸들러 설정. 이러한 핸들러는 사용자 정의 로깅, 복구 시도, 또는 장애에 대한 다른 구성 요소 알림을 구현할 수 있습니다.

5. **문제**: JDK 5 이전의 Java에서 이중 검사 잠금 패턴의 문제점과 `volatile` 키워드가 이러한 문제를 어떻게 해결하는지 설명하세요.
   **해답**: 이중 검사 잠금 패턴(동기화된 블록 전후에 조건 확인)은 지연 초기화에 사용되었습니다. JDK 5 이전에는 다음과 같은 이유로 실패할 수 있었습니다: (1) 적절한 메모리 장벽 없이는 객체가 완전히 구성되기 전에 객체 참조가 다른 스레드에 표시될 수 있음, (2) 컴파일러나 CPU에 의한 명령어 재정렬로 인해 부분적으로 구성된 객체가 사용될 수 있음. 인스턴스 참조에 `volatile`을 추가하면 다음과 같이 이러한 문제를 해결합니다: (a) volatile 필드에 대한 쓰기가 객체 구성 이후에 발생하도록 보장, (b) 재정렬을 방지하는 메모리 장벽 생성, (c) volatile 필드를 읽는 모든 스레드가 초기화 스레드가 수행한 모든 쓰기를 볼 수 있도록 보장. 예: `private volatile static Singleton instance;`

## 스레드 생성 예제

1. **문제**: 다음 POSIX 스레드 생성 코드의 오류를 식별하고 수정하세요:
   ```c
   void *thread_function() {
     printf("Thread running\n");
   }
   
   int main() {
     pthread_t thread;
     pthread_create(thread, NULL, thread_function, NULL);
     pthread_join(thread, NULL);
     return 0;
   }
   ```
   
   **해답**: 여러 오류가 있습니다: (1) `thread_function`이 값을 반환하지 않지만 `void*`를 반환해야 함, (2) `pthread_create`의 첫 번째 인수는 포인터여야 함(`&thread`), (3) 오류 검사가 두 함수 호출 모두에 대해 누락됨. 수정된 코드:
   ```c
   void *thread_function(void *arg) {
     printf("Thread running\n");
     return NULL;
   }
   
   int main() {
     pthread_t thread;
     int status;
     
     status = pthread_create(&thread, NULL, thread_function, NULL);
     if (status != 0) {
       fprintf(stderr, "pthread_create error: %s\n", strerror(status));
       return 1;
     }
     
     status = pthread_join(thread, NULL);
     if (status != 0) {
       fprintf(stderr, "pthread_join error: %s\n", strerror(status));
       return 1;
     }
     
     return 0;
   }
   ```

2. **문제**: Java에서 다음 스레드 생성이 왜 문제를 일으킬 수 있으며, 어떻게 수정하겠습니까?
   ```java
   public class MyThread extends Thread {
     private SharedResource resource;
     
     public MyThread(SharedResource resource) {
       this.resource = resource;
       start();  // 생성자에서 스레드 시작
     }
     
     @Override
     public void run() {
       resource.process();
     }
   }
   ```
   
   **해답**: 생성자에서 스레드를 시작하는 것은 다음과 같은 이유로 문제가 있습니다: (1) 생성자가 완료되기 전에 스레드가 실행을 시작할 수 있어 부분적으로 초기화된 객체에 접근할 가능성이 있음, (2) 서브클래스는 스레드가 시작되기 전에 초기화할 기회가 없음, (3) 생성자가 'this' 참조를 노출해서는 안 된다는 원칙을 위반함. 수정:
   ```java
   public class MyThread extends Thread {
     private SharedResource resource;
     
     public MyThread(SharedResource resource) {
       this.resource = resource;
     }
     
     public void startThread() {
       start();  // 구성 후 이 메서드 호출
     }
     
     @Override
     public void run() {
       resource.process();
     }
   }
   
   // 사용법:
   MyThread thread = new MyThread(resource);
   thread.startThread();
   ```

3. **문제**: 다음 C/POSIX 예제에서 스레드 생성의 차이점은 무엇이며, 각 접근 방식에서 어떤 잠재적 문제가 발생할 수 있습니까?
   
   **예제 1:**
   ```c
   void *thread_func(void *arg) {
     int *value = (int *)arg;
     printf("Value: %d\n", *value);
     return NULL;
   }
   
   int main() {
     int value = 42;
     pthread_t thread;
     pthread_create(&thread, NULL, thread_func, &value);
     pthread_join(thread, NULL);
     return 0;
   }
   ```
   
   **예제 2:**
   ```c
   void *thread_func(void *arg) {
     int *value = (int *)arg;
     printf("Value: %d\n", *value);
     free(value);
     return NULL;
   }
   
   int main() {
     int *value = malloc(sizeof(int));
     *value = 42;
     pthread_t thread;
     pthread_create(&thread, NULL, thread_func, value);
     pthread_join(thread, NULL);
     return 0;
   }
   ```
   
   **해답**: 예제 1은 스택 변수에 대한 포인터를 전달하고, 예제 2는 힙에 메모리를 할당하고 스레드에 소유권을 전달하여 스레드가 이를 해제합니다. 문제점: 예제 1에서는 생성된 스레드가 사용하기 전에 main 스레드가 종료되면 스택 메모리가 유효하지 않을 수 있습니다. 예제 2에서는 스레드가 실행되지 않으면(오류로 인해) 메모리 누수가 발생합니다. 더 나은 접근 방식: (1) 스레드가 main이 종료되기 전에 완료되도록 스레드 조인 사용, (2) 프로세스 수명을 가진 전역 또는 정적 변수 사용, 또는 (3) 적절한 자원 해제를 보장하기 위해 스레드별 정리 핸들러 사용.

4. **문제**: 다음 Java 스레드 생성 접근 방식을 비교하고 차이점을 설명하세요:
   
   **접근 방식 1:**
   ```java
   new Thread(new Runnable() {
     @Override
     public void run() {
       System.out.println("Thread running");
     }
   }).start();
   ```
   
   **접근 방식 2:**
   ```java
   new Thread(() -> System.out.println("Thread running")).start();
   ```
   
   **접근 방식 3:**
   ```java
   Runnable task = () -> System.out.println("Thread running");
   ExecutorService executor = Executors.newSingleThreadExecutor();
   executor.submit(task);
   ```
   
   **해답**: 접근 방식 1은 Runnable을 구현하는 익명 내부 클래스를 사용합니다. 접근 방식 2는 람다 표현식을 사용합니다(접근 방식 1과 기능적으로 동일하지만 더 간결함). 접근 방식 3은 람다가 있는 ExecutorService를 사용합니다. 주요 차이점: (1) 접근 방식 1과 2는 직접 새 스레드를 생성하고 시작하는 반면, 접근 방식 3은 스레드 풀을 사용함, (2) 접근 방식 3은 매번 새 스레드를 만들지 않고 작업 제출을 허용함, (3) 접근 방식 3은 실행자의 명시적 종료가 필요함, (4) 접근 방식 3은 작업 완료 추적을 위한 Future 객체와 같은 더 고급 기능을 제공함. ExecutorService 접근 방식은 일반적으로 더 나은 자원 관리와 성능을 제공하므로 프로덕션 애플리케이션에 선호됩니다.

5. **문제**: 다음 스레드 생성 패턴에서 어떤 문제가 발생할 수 있으며, 어떻게 수정하겠습니까?
   ```c
   void create_worker_threads(int num_threads) {
     pthread_t threads[num_threads];  // 가변 길이 배열
     
     for (int i = 0; i threads = malloc(initial_size * sizeof(pthread_t));
     if (!pool->threads) {
       free(pool);
       return NULL;
     }
     
     pool->capacity = initial_size;
     pool->count = 0;
     pthread_mutex_init(&pool->mutex, NULL);
     return pool;
   }
   
   int pool_add_thread(ThreadPool* pool, pthread_t thread) {
     pthread_mutex_lock(&pool->mutex);
     
     if (pool->count >= pool->capacity) {
       int new_capacity = pool->capacity * 2;
       pthread_t *new_threads = realloc(pool->threads, new_capacity * sizeof(pthread_t));
       if (!new_threads) {
         pthread_mutex_unlock(&pool->mutex);
         return -1;
       }
       pool->threads = new_threads;
       pool->capacity = new_capacity;
     }
     
     pool->threads[pool->count++] = thread;
     pthread_mutex_unlock(&pool->mutex);
     return 0;
   }
   
   void pool_join_all(ThreadPool* pool) {
     pthread_mutex_lock(&pool->mutex);
     int count = pool->count;
     pthread_t* threads = malloc(count * sizeof(pthread_t));
     
     if (!threads) {
       pthread_mutex_unlock(&pool->mutex);
       return;
     }
     
     // 조인 중에 락을 보유하지 않도록 스레드 ID 복사
     memcpy(threads, pool->threads, count * sizeof(pthread_t));
     pthread_mutex_unlock(&pool->mutex);
     
     for (int i = 0; i mutex);
     pool->count = 0;
     pthread_mutex_unlock(&pool->mutex);
   }
   
   void pool_destroy(ThreadPool* pool) {
     pool_join_all(pool);
     pthread_mutex_destroy(&pool->mutex);
     free(pool->threads);
     free(pool);
   }
   ```
   이 구현은 풀에 스레드를 추가하고 한 번에 모두 조인할 수 있도록 합니다.

## 람다 표현식을 사용한 스레드

1. **문제**: Java 스레드 생성에서 다음 람다 표현식의 차이점은 무엇이며, 첫 번째 버전이 어떤 문제를 도입할 수 있을까요?
   
   **버전 1:**
   ```java
   int value = 42;
   new Thread(() -> {
     System.out.println(value);
   }).start();
   value = 43;
   ```
   
   **버전 2:**
   ```java
   int value = 42;
   final int capturedValue = value;
   new Thread(() -> {
     System.out.println(capturedValue);
   }).start();
   value = 43;
   ```
   
   **해답**: 버전 1에서 람다는 `value` 변수를 캡처합니다. 출력은 42일 것입니다(Java에서 람다가 캡처하는 변수는 effectively final이어야 하기 때문에), 그러나 코드에서 즉시 명확하지 않습니다. 만약 스레드가 실행되기 전에 `value`가 수정된다면, 동작이 독자에게 명확하지 않을 것입니다. 버전 2에서는 `value`를 `capturedValue`로 명시적으로 캡처하여 `value`가 변경되더라도 42가 출력될 것이 분명합니다. 버전 1의 문제는 주로 코드 명확성과 유지보수성에 관한 것입니다 – 독자가 출력이 43일 것이라고 잘못 생각할 수 있습니다.

2. **문제**: 스레드와 함께 사용할 때 다음 C++11 람다 캡처 메서드 간의 범위와 수명의 차이점을 설명하세요:
   
   a) `[=]` (값으로 캡처)
   b) `[&]` (참조로 캡처)
   c) `[this]` (this 포인터 캡처)
   
   **해답**: 람다를 스레드와 함께 사용할 때:
   
   a) `[=]`는 변수를 값으로 캡처하여 람다의 수명 동안 존재하는 복사본을 만듭니다. 이는 원래 변수가 변경되더라도 값이 변경되지 않고 원본이 범위를 벗어나더라도 값이 존재하기 때문에 스레딩에 일반적으로 더 안전합니다.
   
   b) `[&]`는 변수를 참조로 캡처하여 람다가 원래 변수에 접근하도록 합니다. 이는 스레드와 함께 사용할 때 위험합니다. 왜냐하면: (1) 참조된 변수가 스레드가 완료되기 전에 범위를 벗어날 수 있어 정의되지 않은 동작이 발생할 수 있고, (2) 참조된 변수가 다른 스레드에 의해 수정될 수 있어 경쟁 조건이 발생할 수 있기 때문입니다.
   
   c) `[this]`는 이 포인터를 캡처하여 클래스 멤버에 접근할 수 있게 합니다. 이는 스레드가 객체보다 오래 지속되거나 여러 스레드가 동기화 없이 동일한 객체 멤버에 접근하는 경우 위험할 수 있습니다.
   
   스레드 안전성을 위해, 동기화로 공유 상태를 명시적으로 관리하지 않는 한 값으로 캡처(`[=]`)가 일반적으로 선호됩니다.

3. **문제**: 이 Java 프로그램은 무엇을 출력하며, 그 이유는 무엇입니까?
   ```java
   public class LambdaThreadTest {
     public static void main(String[] args) throws InterruptedException {
       Counter counter = new Counter();
       
       Runnable incrementer = () -> {
         for (int i = 0; i  threads;
   
   for (int i = 0; i process();
       });
       t.detach();  // 스레드를 Worker 객체에서 분리
     }
     
     ~Worker() {
       std::cout  performTask(), 0, 1, TimeUnit.SECONDS);
   }
   ```
   
   **해답**: 
   
   **접근 방식 1** (수동 대기):
   - 장점: 단순하고 스레드 실행에 대한 직접적인 제어, 이해하기 쉬움.
   - 단점: (1) `performTask()`가 가변 시간이 걸리면 작업 시작 간 기간이 달라짐, (2) 지연을 계산할 때 `performTask()`에 소요된 시간을 고려하지 않음, (3) 중단 수동 처리 필요, (4) 단일 스레드, 각 작업이 완료될 때까지 블록됨.
   
   **접근 방식 2** (ScheduledExecutorService):
   - 장점: (1) 작업 실행 시간에 관계없이 작업 시작 간 고정된 기간 제공, (2) 스레드 풀 관리 자동 처리, (3) 고정 지연 또는 다른 타이밍 패턴으로 쉽게 변경 가능, (4) 더 나은 예외 처리.
   - 단점: (1) 더 복잡함, (2) 작업이 기간보다 오래 걸리면 작업이 대기열에 쌓일 수 있음, (3) 실행자 서비스의 적절한 종료 필요.
   
   두 번째 접근 방식은 더 예측 가능한 타이밍과 내장된 스레드 관리로 인해 프로덕션 애플리케이션에 일반적으로 선호됩니다.

3. **문제**: 다음 C 프로그램은 무엇을 출력하며, 그 이유는 무엇입니까?
   ```c
   #include 
   #include 
   #include 
   
   void *thread_func(void *arg) {
     printf("Thread starting\n");
     sleep(2);
     printf("Thread done\n");
     return NULL;
   }
   
   int main() {
     pthread_t thread;
     printf("Main starting\n");
     pthread_create(&thread, NULL, thread_func, NULL);
     sleep(1);
     printf("Main done\n");
     return 0;
   }
   ```
   
   **해답**: 출력은 일반적으로 다음과 같습니다:
   ```
   Main starting
   Thread starting
   Main done
   ```
   
   프로그램이 종료되기 전에 스레드가 "Thread done"을 출력하지 못할 수 있습니다. 이는 다음과 같은 이유로 발생합니다: (1) 메인 스레드가 새 스레드를 생성한 다음 1초 동안 대기함, (2) 이 동안 새 스레드가 시작되어 시작 메시지를 출력한 다음 2초 동안 대기하기 시작함, (3) 메인 스레드는 1초 후에 깨어나 "Main done"을 출력한 다음 프로그램을 종료함, (4) 메인 스레드가 종료되면 실행 중인 스레드를 포함한 전체 프로세스가 종료됨. 이는 생성된 스레드가 2초 대기를 완료하기 전에 발생합니다. 이를 수정하려면 main에서 반환하기 전에 `pthread_join(thread, NULL)` 호출을 추가하세요.

4. **문제**: 멀티스레드 애플리케이션에서 아래와 같이 스레드 간 동기화를 위해 sleep()을 사용하는 것의 문제점은 무엇입니까?
   ```java
   // 스레드 1
   public void thread1() {
     prepareData();
     dataReady = true;
   }
   
   // 스레드 2
   public void thread2() {
     Thread.sleep(500);  // 스레드 1이 데이터를 준비할 때까지 대기
     if (dataReady) {
       processData();
     }
   }
   ```
   
   **해답**: 스레드 동기화에 sleep()을 사용하는 것은 다음과 같은 이유로 신뢰할 수 없습니다: (1) 스레드 1이 항상 500ms 내에 데이터 준비를 완료한다고 가정하지만, 모든 시스템 부하나 조건에서 사실이 아닐 수 있음, (2) 스레드 1이 일찍 완료하면 CPU 시간을 낭비함, (3) 다른 하드웨어나 JVM 구현에서 유지되지 않을 수 있는 타이밍 가정에 의존하는 취약한 코드를 만듦. 더 나은 접근 방식은:
   
   명시적 동기화 사용:
   ```java
   private final Object lock = new Object();
   private boolean dataReady = false;
   
   // 스레드 1
   public void thread1() {
     prepareData();
     synchronized (lock) {
       dataReady = true;
       lock.notifyAll();
     }
   }
   
   // 스레드 2
   public void thread2() {
     synchronized (lock) {
       while (!dataReady) {
         try {
           lock.wait();
         } catch (InterruptedException e) {
           Thread.currentThread().interrupt();
           return;
         }
       }
     }
     processData();
   }
   ```
   
   또는 더 높은 수준의 동시성 유틸리티 사용:
   ```java
   private final CountDownLatch dataReadyLatch = new CountDownLatch(1);
   
   // 스레드 1
   public void thread1() {
     prepareData();
     dataReadyLatch.countDown();
   }
   
   // 스레드 2
   public void thread2() {
     try {
       dataReadyLatch.await();
       processData();
     } catch (InterruptedException e) {
       Thread.currentThread().interrupt();
     }
   }
   ```

5. **문제**: sleep()을 사용할 때 스레드 우선순위의 중요성은 무엇이며, 다음 코드는 다른 시스템에서 어떻게 다르게 동작할 수 있을까요?
   ```java
   public class ThreadPriorityTest {
     public static void main(String[] args) {
       Thread highPriority = new Thread(() -> {
         for (int i = 0; i  {
         for (int i = 0; i  {
       processTask(taskId);
     });
   }
   
   executor.shutdown();
   ```
   
   작업 중 하나가 처리되지 않은 예외를 발생시키면 어떻게 될까요? 작업 실패를 감지하고 처리하도록 이 코드를 어떻게 수정하겠습니까?
   
   **해답**: 작업이 처리되지 않은 예외를 발생시키면: (1) 예외는 실행자 프레임워크에 의해 포착됨, (2) 작업을 실행하고 있던 스레드는 여전히 다른 작업을 실행할 수 있음, (3) Future 객체를 사용하여 실행을 추적하지 않는 한 예외 세부 정보가 손실됨.
   
   실패를 감지하도록 수정된 버전:
   ```java
   ExecutorService executor = Executors.newFixedThreadPool(4);
   List> futures = new ArrayList<>();
   
   for (int i = 0; i < 100; i++)
```