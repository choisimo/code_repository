# 운영체제 프로세스 관리 심화 문제집: 개념 및 실전 문제

프로세스 관리는 운영체제의 핵심 기능 중 하나로, 다양한 개념과 메커니즘을 포함하고 있습니다. 이 문서에서는 프로세스 스케줄링, 상태 전이, 프로세스 생성 등 주요 개념별로 헷갈리기 쉬운 부분들을 다루는 심화 문제들을 제공합니다. 각 주제별로 5개 이상의 문제를 통해 개념을 심층적으로 학습하고 실제 적용 능력을 향상시킬 수 있습니다.

## 1. 프로세스 스케줄링 원칙 (Process Scheduling Principles)

### 문제 1.1: CPU 스케줄링의 주요 목표로 옳지 않은 것은?
a) CPU 이용률 최대화
b) 처리량(throughput) 최대화
c) 응답 시간 최소화
d) 처리 시간의 편차 최대화

**정답:** d) 처리 시간의 편차 최대화

**해설:** CPU 스케줄링의 목표는 CPU 이용률과 처리량을 최대화하고, 총처리 시간, 대기 시간, 응답 시간을 최소화하는 것입니다. 처리 시간의 편차는 최소화하는 것이 바람직합니다. 특히 대화식 시스템에서는 응답 시간의 편차를 줄이는 것이 중요합니다[2].

### 문제 1.2: 다음 중 선점형(preemptive) 스케줄링에 대한 설명으로 올바른 것은?
a) 프로세스가 종료되거나 I/O 요청이 발생할 때까지 CPU를 계속 사용한다
b) 높은 우선순위의 프로세스가 도착하면 현재 실행 중인 프로세스를 중단시킬 수 있다
c) 문맥 교환(context switching) 오버헤드가 적다
d) 실시간 시스템에 부적합하다

**정답:** b) 높은 우선순위의 프로세스가 도착하면 현재 실행 중인 프로세스를 중단시킬 수 있다

**해설:** 선점형 스케줄링은 우선순위가 높은 프로세스가 도착하면 현재 실행 중인 프로세스를 중단시키고 CPU를 할당받을 수 있습니다. 이는 실시간 시스템에 적합하지만, 문맥 교환 오버헤드가 크고 데이터 공유 시 Race Condition 문제가 발생할 수 있습니다[2].

### 문제 1.3: 비선점형(non-preemptive) 스케줄링의 주요 특징으로 옳은 것은?
a) 높은 우선순위 프로세스가 항상 즉시 실행된다
b) 한번 CPU를 할당받으면 I/O 요청이나 종료 전까지 계속 사용한다
c) 모든 프로세스에 공평한 CPU 시간을 제공한다
d) 문맥 교환(context switching)이 자주 발생한다

**정답:** b) 한번 CPU를 할당받으면 I/O 요청이나 종료 전까지 계속 사용한다

**해설:** 비선점형 스케줄링은 프로세스가 CPU를 할당받으면 I/O 요청이나 프로세스 종료 전까지 CPU를 계속 사용합니다. 이로 인해 높은 우선순위 프로세스도 현재 실행 중인 프로세스가 끝날 때까지 기다려야 합니다[1].

### 문제 1.4: CPU 스케줄링에서 '디스패치 지연(dispatch latency)'의 정의는?
a) 프로세스가 준비 상태에서 실행 상태로 전환되는 시간
b) 디스패처가 한 프로세스를 정지하고 다른 프로세스의 수행을 시작하는데 소요되는 시간
c) I/O 요청 후 완료될 때까지 걸리는 시간
d) 프로세스 생성 시 초기화에 필요한 시간

**정답:** b) 디스패처가 한 프로세스를 정지하고 다른 프로세스의 수행을 시작하는데 소요되는 시간

**해설:** 디스패치 지연(dispatch latency)은 CPU 스케줄러가 하나의 프로세스를 정지하고 다른 프로세스의 수행을 시작하는데까지 소요되는 시간을 의미합니다[2].

### 문제 1.5: 대화식 시스템(interactive system)에서 가장 중요한 CPU 스케줄링 평가 기준은?
a) 처리량(throughput)
b) 응답 시간(response time)
c) CPU 이용률(CPU utilization)
d) 총 처리 시간(turnaround time)

**정답:** b) 응답 시간(response time)

**해설:** 대화식 시스템에서는 사용자와의 상호작용이 중요하므로 응답 시간이 가장 중요한 평가 기준입니다. 특히 평균 응답 시간보다 응답 시간의 편차(variance)를 최소화하는 것이 더 중요할 수 있습니다[2].

## 2. CPU 스케줄링 전략 (CPU Scheduling Strategies)

### 문제 2.1: SJF(Shortest Job First) 스케줄링 알고리즘에 대한 설명으로 옳지 않은 것은?
a) 평균 대기 시간을 최소화한다
b) 선점형과 비선점형으로 구현 가능하다
c) 프로세스의 실행 시간을 미리 알아야 한다
d) FCFS(First-Come, First-Served)보다 평균 대기 시간이 항상 길다

**정답:** d) FCFS(First-Come, First-Served)보다 평균 대기 시간이 항상 길다

**해설:** SJF 알고리즘은 평균 대기 시간을 최소화하는 알고리즘이므로, FCFS보다 평균 대기 시간이 짧습니다. 다만 실제 구현에서는 프로세스의 실행 시간을 미리 정확히 알기 어렵다는 한계가 있습니다[2].

### 문제 2.2: 다음 프로세스 정보에 대해 SJF 스케줄링을 적용할 때 실행 순서는?

|프로세스|도착시간|실행시간|
|--|--|--|
|P1|0|30|
|P2|5|10|
|P3|10|15|
|P4|15|10|

a) P1 → P2 → P3 → P4
b) P2 → P4 → P3 → P1
c) P1 → P2 → P4 → P3
d) P1 → P3 → P2 → P4

**정답:** c) P1 → P2 → P4 → P3

**해설:** SJF는 비선점형이므로 한번 실행되면 중간에 중단될 수 없습니다. P1이 가장 먼저 도착하여 실행(0~30), P2가 다음으로 실행(30~40), 이후 P3와 P4 중에서 실행 시간이 짧은 P4가 실행(40~50), 마지막으로 P3이 실행(50~65)됩니다[1].

### 문제 2.3: HRN(Highest Response-ratio Next) 스케줄링에서 우선순위를 계산하는 공식은?
a) (대기시간 + 서비스시간) / 서비스시간
b) 대기시간 / 서비스시간
c) 서비스시간 / 대기시간
d) (대기시간 × 서비스시간) / (대기시간 + 서비스시간)

**정답:** a) (대기시간 + 서비스시간) / 서비스시간

**해설:** HRN 스케줄링은 우선순위를 (대기시간 + 서비스시간) / 서비스시간 공식으로 계산하며, 이 값이 클수록 우선순위가 높습니다. 이 방식은 대기 시간이 길거나 서비스 시간이 짧은 프로세스에게 유리합니다[7].

### 문제 2.4: 다음 프로세스 정보를 바탕으로 HRN 방식으로 스케줄링할 경우 우선순위가 가장 높은 프로세스는?

|작업|대기시간|서비스시간|
|--|--|--|
|A|5|20|
|B|40|20|
|C|15|45|
|D|20|20|

a) A
b) B
c) C
d) D

**정답:** b) B

**해설:** 
- A: (5+20)/20 = 1.25
- B: (40+20)/20 = 3.0
- C: (15+45)/45 = 1.33
- D: (20+20)/20 = 2.0
최고응답률이 가장 큰 프로세스는 B(3.0)이므로 B가 가장 높은 우선순위를 갖습니다[7].

### 문제 2.5: 다단계 피드백 큐(MLFQ) 스케줄링의 특징으로 옳지 않은 것은?
a) 여러 개의 큐를 사용하며 각 큐는 서로 다른 우선순위를 갖는다
b) 프로세스의 우선순위는 과거 실행 패턴에 따라 동적으로 변경된다
c) 높은 우선순위 큐의 프로세스는 낮은 우선순위 큐의 프로세스보다 먼저 실행된다
d) 모든 프로세스는 항상 동일한 시간 할당량(time quantum)을 갖는다

**정답:** d) 모든 프로세스는 항상 동일한 시간 할당량(time quantum)을 갖는다

**해설:** MLFQ에서는 일반적으로 우선순위가 낮은 큐일수록 시간 할당량을 더 길게 설정합니다. 이는 CPU를 많이 사용하는 프로세스가 낮은 우선순위 큐로 이동했을 때 문맥 교환 오버헤드를 줄이기 위함입니다[3].

## 3. 스케줄링 큐 관리 (Scheduling Queues Management)

### 문제 3.1: 다음 중 프로세스 스케줄링에서 사용되는 큐가 아닌 것은?
a) 준비 큐(Ready Queue)
b) 대기 큐(Wait Queue)
c) 실행 큐(Running Queue)
d) 장치 큐(Device Queue)

**정답:** c) 실행 큐(Running Queue)

**해설:** 프로세스 스케줄링에서는 준비 큐(Ready Queue), 대기 큐(Wait Queue), 장치 큐(Device Queue) 등이 사용되지만, '실행 큐'라는 개념은 없습니다. 실행(Running) 상태는 한 번에 하나의 프로세스만 가능하기 때문에 큐 형태가 아닙니다[4].

### 문제 3.2: 다단계 피드백 큐(MLFQ) 스케줄링에서 프로세스의 우선순위를 결정하는 기준으로 옳은 것은?
a) 프로세스의 도착 시간만을 고려한다
b) 프로세스의 실행 시간만을 고려한다
c) 프로세스의 과거 실행 패턴을 기반으로 우선순위를 동적으로 변경한다
d) 프로세스의 우선순위는 한 번 설정되면 변경되지 않는다

**정답:** c) 프로세스의 과거 실행 패턴을 기반으로 우선순위를 동적으로 변경한다

**해설:** MLFQ는 프로세스의 과거 실행 패턴에 따라 우선순위를 동적으로 변경합니다. 예를 들어, I/O 작업을 자주 발생시키는 대화형 프로그램의 경우 우선순위를 높게 설정하고, CPU를 많이 사용하는 작업은 우선순위를 낮게 설정합니다[3].

### 문제 3.3: Solaris 운영체제의 MLFQ 구현에서 큐의 수는 대략 몇 개인가?
a) 10개
b) 60개
c) 170개
d) 256개

**정답:** c) 170개

**해설:** Solaris 운영체제에서 사용되는 MLFQ 구현은 약 170개의 큐를 사용합니다. 이 중 0~60번 큐는 사용자 작업에 사용되며, 우선순위에 따라 time slice 길이도 다르게 설정됩니다[3].

### 문제 3.4: MLFQ에서 'priority boost'의 주요 목적은 무엇인가?
a) 높은 우선순위 작업을 더 빠르게 실행하기 위함
b) 낮은 우선순위로 내려간 작업들의 기아 상태(starvation)를 방지하기 위함
c) CPU 이용률을 높이기 위함
d) 문맥 교환 오버헤드를 줄이기 위함

**정답:** b) 낮은 우선순위로 내려간 작업들의 기아 상태(starvation)를 방지하기 위함

**해설:** MLFQ에서는 일정 시간마다 모든 작업의 우선순위를 최상위 큐로 올려주는 'priority boost' 메커니즘을 사용합니다. 이는 낮은 우선순위로 내려간 작업들이 영원히 실행되지 못하는 기아 상태를 방지하기 위함입니다[3].

### 문제 3.5: 프로세스 스케줄링 큐의 구현에 가장 적합한 자료구조는?
a) 스택(Stack)
b) 연결 리스트(Linked List)
c) 해시 테이블(Hash Table)
d) 이진 검색 트리(Binary Search Tree)

**정답:** b) 연결 리스트(Linked List)

**해설:** 프로세스 스케줄링 큐는 일반적으로 연결 리스트로 구현됩니다. 이는 삽입과 삭제가 빈번하게 발생하는 스케줄링 큐의 특성에 적합하며, 우선순위 큐, FIFO 큐 등 다양한 형태로 구현될 수 있습니다[2].

## 4. 준비 큐와 대기 큐 작업 (Ready Queue and Wait Queue Operations)

### 문제 4.1: 다음 중 프로세스가 준비 큐(Ready Queue)에서 대기 큐(Wait Queue)로 이동하는 경우는?
a) 프로세스가 I/O 요청을 했을 때
b) 프로세스가 CPU 시간 할당량을 모두 사용했을 때
c) 프로세스가 생성되었을 때
d) 높은 우선순위의 프로세스가 도착했을 때

**정답:** a) 프로세스가 I/O 요청을 했을 때

**해설:** 프로세스가 I/O 요청 등의 이벤트가 발생하면 실행 상태에서 대기(blocked) 상태로 전환되며, 이때 프로세스는 준비 큐에서 대기 큐로 이동합니다[4].

### 문제 4.2: 싱글 코어 CPU 시스템에서 동시에 Running 상태에 있을 수 있는 프로세스 수는?
a) 0개
b) 1개
c) 다수의 프로세스
d) CPU 코어 수에 따라 다르다

**정답:** b) 1개

**해설:** 싱글 코어 CPU 시스템에서는 한 순간에 오직 하나의 프로세스만 실행(Running) 상태에 있을 수 있습니다. Ready, Blocked 상태에는 여러 프로세스가 존재할 수 있지만, Running 상태의 프로세스는 단 하나만 존재합니다[4].

### 문제 4.3: 프로세스가 대기 큐(Wait Queue)에서 준비 큐(Ready Queue)로 이동하는 경우는?
a) 프로세스가 CPU를 할당받았을 때
b) 프로세스가 I/O 작업을 완료했을 때
c) 프로세스가 다른 프로세스를 생성했을 때
d) 프로세스가 종료되었을 때

**정답:** b) 프로세스가 I/O 작업을 완료했을 때

**해설:** 프로세스가 I/O 작업이나 기타 이벤트 완료 후, 대기(blocked) 상태에서 준비(ready) 상태로 전환되며 대기 큐에서 준비 큐로 이동합니다[4].

### 문제 4.4: 준비 큐(Ready Queue)에 있는 프로세스들의 특징으로 옳지 않은 것은?
a) CPU를 할당받기 위해 대기 중이다
b) 모든 자원을 할당받은 상태이다
c) 실행 가능한 상태이다
d) 여러 개의 프로세스가 동시에 있을 수 있다

**정답:** b) 모든 자원을 할당받은 상태이다

**해설:** 준비 큐에 있는 프로세스는 CPU를 제외한 모든 필요한 자원을 할당받은 상태입니다. 즉, CPU만 할당받으면 즉시 실행될 수 있는 상태이지만, 모든 자원을 할당받은 것은 아닙니다[4].

### 문제 4.5: 대기 큐(Wait Queue)의 특징으로 옳은 것은?
a) 단일 대기 큐만 존재한다
b) I/O 장치별로 별도의 대기 큐가 존재할 수 있다
c) 대기 큐에서 준비 큐로 직접 이동하지 않고 항상 실행 상태를 거친다
d) 우선순위가 높은 프로세스가 먼저 대기 큐에서 나간다

**정답:** b) I/O 장치별로 별도의 대기 큐가 존재할 수 있다

**해설:** 시스템에는 여러 개의 대기 큐가 존재할 수 있으며, 각 I/O 장치나 이벤트마다 별도의 대기 큐를 가질 수 있습니다. 프로세스는 해당 이벤트가 완료되면 대기 큐에서 준비 큐로 이동합니다[2].

## 5. 컨텍스트 스위치 정의 및 메커니즘 (Context Switch Definition and Mechanism)

### 문제 5.1: 컨텍스트 스위칭(Context Switching)의 정확한 정의는?
a) 프로세스 간 통신 방법
b) CPU에서 실행 중이던 프로세스가 다른 프로세스로 교체되는 것
c) 프로세스의 상태 정보를 저장하는 데이터 구조
d) 프로세스가 생성되는 과정

**정답:** b) CPU에서 실행 중이던 프로세스가 다른 프로세스로 교체되는 것

**해설:** 컨텍스트 스위칭은 CPU/코어에서 실행 중이던 프로세스/스레드가 다른 프로세스/스레드로 교체되는 것을 의미합니다. 이때 이전 프로세스의 상태(컨텍스트)를 저장하고, 새로운 프로세스의 상태를 복원하는 작업이 수행됩니다[5].

### 문제 5.2: 컨텍스트 스위칭이 발생하는 상황이 아닌 것은?
a) 프로세스의 Time Slice가 만료되었을 때
b) I/O 작업이 필요할 때
c) 인터럽트가 발생했을 때
d) 프로세스가 메모리에 로드될 때

**정답:** d) 프로세스가 메모리에 로드될 때

**해설:** 컨텍스트 스위칭은 주어진 Time Slice를 다 사용했을 때, I/O 작업이 필요할 때, 다른 리소스를 기다려야 할 때, 인터럽트가 발생했을 때 등에 발생합니다. 프로세스가 메모리에 로드되는 것은 컨텍스트 스위칭의 트리거가 아닙니다[5].

### 문제 5.3: 프로세스 컨텍스트 스위칭과 스레드 컨텍스트 스위칭의 주요 차이점은?
a) 프로세스 컨텍스트 스위칭은 사용자 모드에서 실행되고, 스레드 컨텍스트 스위칭은 커널 모드에서 실행된다
b) 프로세스 컨텍스트 스위칭은 가상 메모리 주소 공간 전환이 필요하지만, 같은 프로세스 내 스레드 간 스위칭은 그렇지 않다
c) 스레드 컨텍스트 스위칭은 CPU 레지스터 상태를 저장하지 않는다
d) 프로세스 컨텍스트 스위칭은 인터럽트에 의해서만 발생한다

**정답:** b) 프로세스 컨텍스트 스위칭은 가상 메모리 주소 공간 전환이 필요하지만, 같은 프로세스 내 스레드 간 스위칭은 그렇지 않다

**해설:** 프로세스 컨텍스트 스위칭과 스레드 컨텍스트 스위칭의 가장 큰 차이점은 가상 메모리 주소 관련 처리입니다. 프로세스 간 스위칭은 메모리 주소 공간을 완전히 전환해야 하지만, 같은 프로세스 내 스레드 간 스위칭은 메모리 공간을 공유하므로 이러한 전환이 필요하지 않습니다[5].

### 문제 5.4: 컨텍스트 스위칭의 오버헤드에 포함되지 않는 것은?
a) CPU 레지스터 상태 저장 및 복원
b) 메모리 매핑 정보(TLB) 갱신
c) CPU 캐시 오염(cache pollution)
d) 네트워크 연결 재설정

**정답:** d) 네트워크 연결 재설정

**해설:** 컨텍스트 스위칭의 오버헤드에는 CPU 레지스터 상태 저장 및 복원, 메모리 매핑 정보 갱신, CPU 캐시 오염 등이 포함되지만, 네트워크 연결 재설정은 포함되지 않습니다. 네트워크 연결은 프로세스 수준에서 유지되며 컨텍스트 스위칭과 직접적인 관련이 없습니다[5].

### 문제 5.5: 컨텍스트 스위칭이 실행되는 모드는?
a) 사용자 모드(User Mode)
b) 커널 모드(Kernel Mode)
c) 특권 모드(Privileged Mode)
d) 하이퍼바이저 모드(Hypervisor Mode)

**정답:** b) 커널 모드(Kernel Mode)

**해설:** 컨텍스트 스위칭은 항상 커널 모드에서 실행됩니다. 컨텍스트 스위칭은 OS의 커널에 의해 수행되며, CPU 레지스터와 메모리 상태와 같은 중요한 시스템 리소스에 접근해야 하기 때문에 높은 권한이 필요합니다[5].

## 6. 프로세스 상태 전이도 (Process State Transitions Diagram)

### 문제 6.1: 프로세스 상태 전이도에서 기본적으로 표현되는 5가지 상태가 아닌 것은?
a) New(생성)
b) Ready(준비)
c) Running(실행)
d) Sleeping(수면)

**정답:** d) Sleeping(수면)

**해설:** 프로세스 상태 전이도에서 기본적으로 표현되는 5가지 상태는 New(생성), Ready(준비), Running(실행), Blocked/Waiting(대기), Terminated(종료)입니다. Sleeping은 일반적인 프로세스 상태 모델에 포함되지 않습니다[6].

### 문제 6.2: 프로세스가 'New' 상태에서 'Ready' 상태로 전이되는 과정을 무엇이라고 하는가?
a) 디스패치(Dispatch)
b) 스케줄링(Scheduling)
c) 어드미션(Admission)
d) 초기화(Initialization)

**정답:** c) 어드미션(Admission)

**해설:** 프로세스가 'New' 상태에서 'Ready' 상태로 전이되는 과정을 어드미션(Admission)이라고 합니다. 이는 운영체제가 새로 생성된 프로세스를 시스템에 받아들이고 준비 큐에 배치하는 과정입니다[4].

### 문제 6.3: 프로세스가 'Running' 상태에서 'Ready' 상태로 전이되는 경우로 옳지 않은 것은?
a) 시간 할당량(Time Quantum)이 만료되었을 때
b) 높은 우선순위의 프로세스가 도착했을 때(선점 스케줄링의 경우)
c) 프로세스가 I/O 작업을 요청했을 때
d) 인터럽트가 발생했을 때

**정답:** c) 프로세스가 I/O 작업을 요청했을 때

**해설:** 프로세스가 I/O 작업을 요청하면 'Running' 상태에서 'Blocked' 상태로 전이됩니다. 'Running'에서 'Ready'로의 전이는 시간 할당량 만료, 높은 우선순위 프로세스 도착(선점 스케줄링), 인터럽트 등의 경우에 발생합니다[4].

### 문제 6.4: 다음 중 프로세스가 'Ready' 상태에서 'Running' 상태로 전이되는 이벤트를 무엇이라고 하는가?
a) 스케줄링(Scheduling)
b) 디스패치(Dispatch)
c) 프리엠션(Preemption)
d) 웨이크업(Wakeup)

**정답:** b) 디스패치(Dispatch)

**해설:** 준비 상태(Ready)에 있는 프로세스가
 실행 상태(Running)로 전이되는 이벤트를 디스패치(Dispatch)라고 합니다. 이는 CPU 스케줄러가 선택한 프로세스에게 CPU 제어권을 넘겨주는 과정입니다[2].

### 문제 6.5: 프로세스 상태 전이도에서 'Blocked' 상태에서 직접 'Running' 상태로 전이가 가능한가?
a) 예, 항상 가능하다
b) 아니오, 'Blocked' 상태에서는 항상 'Ready' 상태를 거쳐야만 'Running' 상태가 될 수 있다
c) 특정 스케줄링 알고리즘에서만 가능하다
d) I/O 작업이 완료된 직후에만 가능하다

**정답:** b) 아니오, 'Blocked' 상태에서는 항상 'Ready' 상태를 거쳐야만 'Running' 상태가 될 수 있다

**해설:** 프로세스 상태 전이도에서 'Blocked' 상태의 프로세스는 대기 중인 이벤트(예: I/O 완료)가 발생하면 'Ready' 상태로 이동하고, 이후 CPU 스케줄러에 의해 선택되면 'Running' 상태가 됩니다. 'Blocked'에서 직접 'Running'으로의 전이는 불가능합니다[4].

## 7. 프로세스 생성 메커니즘 (Process Creation Mechanisms)

### 문제 7.1: 유닉스/리눅스 시스템에서 프로세스 생성에 주로 사용되는 시스템 콜은?
a) create()
b) fork()
c) new()
d) process()

**정답:** b) fork()

**해설:** 유닉스/리눅스 시스템에서는 fork() 시스템 콜을 통해 새로운 프로세스를 생성합니다. fork()는 호출한 프로세스(부모 프로세스)를 복제하여 새로운 프로세스(자식 프로세스)를 만듭니다[8].

### 문제 7.2: 프로세스 생성 과정에서 일어나는 일로 옳지 않은 것은?
a) 고유한 프로세스 ID 할당
b) 프로세스 제어 블록(PCB) 생성
c) 메모리 공간 할당
d) 모든 레지스터 값을 0으로 초기화

**정답:** d) 모든 레지스터 값을 0으로 초기화

**해설:** 프로세스 생성 과정에서 스택 포인터와 프로그램 카운터를 제외한 많은 레지스터 값이 0으로 초기화될 수 있지만, 모든 레지스터 값이 0으로 초기화되는 것은 아닙니다. 특히 프로그램 카운터는 프로그램의 시작점 주소로 초기화됩니다[8].

### 문제 7.3: 새로운 프로세스가 생성될 때 초기 상태로 설정되는 것은?
a) Ready
b) Running
c) New
d) Blocked

**정답:** c) New

**해설:** 새로운 프로세스가 생성되면 초기 상태는 'New'로 설정됩니다. 이후 프로세스를 스케줄링 큐에 넣고 상태를 'Ready'로 변경합니다[8].

### 문제 7.4: 유닉스/리눅스 시스템에서 부팅 시 생성되는 프로세스가 아닌 것은?
a) init
b) swapper
c) pagedaemon
d) shell

**정답:** d) shell

**해설:** 유닉스/리눅스 시스템에서 부팅 시 Swapper, Pagedaemon, Init 이렇게 3개의 프로세스가 먼저 생성됩니다. 이들은 운영체제(커널) 프로세스입니다. shell은 사용자 로그인 후 생성되는 프로세스입니다[8].

### 문제 7.5: 프로세스 생성 과정에서 PCB(Process Control Block)에 저장되는 정보로 옳지 않은 것은?
a) 프로세스 ID
b) 프로세스 상태
c) 다른 프로세스의 메모리 주소
d) CPU 레지스터 값

**정답:** c) 다른 프로세스의 메모리 주소

**해설:** PCB(Process Control Block)에는 프로세스 ID, 상태, CPU 레지스터 값, 프로그램 카운터, 메모리 관리 정보, 스케줄링 정보 등이 저장되지만, 다른 프로세스의 메모리 주소는 저장되지 않습니다. 각 프로세스는 자신의 메모리 공간에 대한 정보만 PCB에 저장합니다[8].

## 8. 프로세스 관계와 시스템 콜 (Process Relationships and System Calls)

### 문제 8.1: fork() 시스템 콜 호출 후 반환 값에 대한 설명으로 옳은 것은?
a) 부모 프로세스: 자식 프로세스의 PID, 자식 프로세스: 0
b) 부모 프로세스: 0, 자식 프로세스: 부모 프로세스의 PID
c) 부모 프로세스와 자식 프로세스 모두 동일한 PID 반환
d) 성공 시 1, 실패 시 -1 반환

**정답:** a) 부모 프로세스: 자식 프로세스의 PID, 자식 프로세스: 0

**해설:** fork() 시스템 콜이 성공하면 부모 프로세스에는 자식 프로세스의 PID가 반환되고, 자식 프로세스에는 0이 반환됩니다. 이를 통해 코드 내에서 부모 프로세스와 자식 프로세스를 구분할 수 있습니다[8].

### 문제 8.2: 다음 코드를 실행하면 몇 개의 프로세스가 생성되는가?
```c
#include 
#include 

int main() {
    fork();
    fork();
    fork();
    printf("Hello\n");
    return 0;
}
```
a) 3개
b) 4개
c) 7개
d) 8개

**정답:** d) 8개

**해설:** 각 fork() 호출은 현재 존재하는 모든 프로세스에 대해 복제본을 생성합니다. 첫 번째 fork()로 2개, 두 번째 fork()로 4개, 세 번째 fork()로 8개의 프로세스가 생성됩니다. 따라서 총 8개의 프로세스가 생성되고, "Hello"가 8번 출력됩니다.

### 문제 8.3: exec() 시스템 콜 패밀리의 주요 기능은?
a) 새로운 프로세스 생성
b) 현재 프로세스의 이미지를 새로운 프로그램으로 대체
c) 프로세스 종료
d) 프로세스 간 통신

**정답:** b) 현재 프로세스의 이미지를 새로운 프로그램으로 대체

**해설:** exec() 시스템 콜 패밀리는 현재 실행 중인 프로세스의 이미지를 새로운 프로그램으로 완전히 대체합니다. 프로세스 ID는 유지되지만, 코드, 데이터, 스택 등 프로세스의 내용이 새로운 프로그램으로 변경됩니다[8].

### 문제 8.4: wait() 시스템 콜의 주요 기능은?
a) 자식 프로세스가 종료될 때까지 부모 프로세스를 대기 상태로 만든다
b) 특정 시간 동안 프로세스를 일시 중지한다
c) I/O 작업이 완료될 때까지 프로세스를 대기 상태로 만든다
d) 다른 프로세스와 동기화하기 위해 사용한다

**정답:** a) 자식 프로세스가 종료될 때까지 부모 프로세스를 대기 상태로 만든다

**해설:** wait() 시스템 콜은 자식 프로세스가 종료될 때까지 부모 프로세스를 대기 상태로 만듭니다. 자식 프로세스가 종료되면 wait()는 종료된 자식 프로세스의 PID와 종료 상태를 반환합니다.

### 문제 8.5: 좀비(Zombie) 프로세스가 발생하는 원인은?
a) 자식 프로세스가 종료되었지만, 부모 프로세스가 wait() 호출을 하지 않아 프로세스 테이블에 남아있는 경우
b) 부모 프로세스가 자식 프로세스보다 먼저 종료되어 자식 프로세스가 고아가 된 경우
c) 프로세스가 무한 루프에 빠져 종료되지 않는 경우
d) 프로세스가 메모리 부족으로 정지된 경우

**정답:** a) 자식 프로세스가 종료되었지만, 부모 프로세스가 wait() 호출을 하지 않아 프로세스 테이블에 남아있는 경우

**해설:** 좀비 프로세스는 자식 프로세스가 종료되었지만, 부모 프로세스가 wait() 또는 waitpid() 시스템 콜을 호출하지 않아 자식 프로세스의 종료 상태가 수집되지 않은 경우 발생합니다. 이로 인해 프로세스 테이블에 항목이 계속 남게 됩니다.

## 결론

프로세스 스케줄링과 관리는 운영체제의 핵심 기능으로, 이러한 개념을 정확히 이해하는 것은 시스템 프로그래밍과 운영체제 설계에 있어 매우 중요합니다. 이번 문제집을 통해 프로세스 스케줄링 원칙, CPU 스케줄링 전략, 프로세스 상태 전이, 컨텍스트 스위치, 프로세스 생성 및 관리 등의 개념을 다양한 측면에서 학습했습니다.

특히 SJF, HRN, MLFQ 등 다양한 스케줄링 알고리즘의 특징과 차이점, 프로세스 상태 간 전이 조건, 컨텍스트 스위칭의 메커니즘, fork()와 exec() 시스템 콜의 동작 방식 등은 실제 시스템에서 프로세스가 어떻게 관리되는지 이해하는 데 필수적인 개념입니다.

이러한 개념들은 단순히 이론적인 지식에 그치지 않고, 실제 시스템 설계와 성능 최적화에 직접적으로 적용됩니다. 따라서 이번 문제집을 통해 습득한 지식을 바탕으로 실제 시스템에서의 프로세스 관리 방식을 분석하고 개선하는 능력을 기를 수 있을 것입니다.
