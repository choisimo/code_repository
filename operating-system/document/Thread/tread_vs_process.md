## Resource requirements difference between threads and processes

### Thread
-   스레드 실벽자 (TID)
-   REGISTER 세트 (프로그램카운터, 스택 포인터 등 CPU 하드웨어 레지스터)
-   스택 (로컬 변수와 함수 호출 전용) -> 스레드 간에 공유 안 되는 부분
-   Thread Control Block (TCB)
-   스레드별 데이터 (Thread-Local Storage)

### Thread 의 Stack 과 Thread Local Storage 에 관하여..
-   스레드는 프로세스 내에서 실행되는 흐름의 단위로, 실행될 때 다음과 같은 독립적인 리소스를 가진다. 
1. 스택 (Stack)
-   함수 호출 정보와 지역 변수를 저장하는 메모리 영역
-   함수가 호출될 때 생성되고 함수가 종료되면 해제되는 불안정 공간
-   LIFO 구조
-   스레드마다 독립적인 공간으로 할당
2. TLs
-   스레드별로 독립적인 저장 공간
-   전역 변수처럼 사용 가능하나, 해당 스레드만 접근 가능
-   스레드의 생명주기 동안 유지되는 데이터를 저장
-   경합없이 데이터 관리 가능

### Process
-   프로세스 식별자 (PID)
-   메모리 공간 (텍스트, 데이터, 힙 세그먼트)
-   Process Control Block (PCB)
-   FileDescriptor Table
-   보안 컨텍스트 (사용자 ID, 그룹 ID)
-   최소 하나의 스레드 (메인 스레드)

### 비교
-   동일 프로세스 내의 스레드들은 메모리 공간, 파일 디스크립터, 보안 컨텍스트 등 대부분의 리소를 공유하는 반면, 각 프로세스는 이러한 리소스의 개별 사본을 가진다. 이로 인해 스레드 생성이 프로세스 생성보다 더 빠르고 리소스 효율적이다.