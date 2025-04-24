## Context-switching between Kernel Threads

### processing 
1. **상태 저장**: 현재 스레드의 실행 컨텍스트 
(레지스터, PC, stack ptr 등)을 Thread Control Block 에 저장함

2. **스케줄링** : 스케줄러를 통해 다음에 실행할 스레드 선택

3. **상태 복원** : 선택된 스레드의 컨텍스트를 해당 TCB 에서 로드함

4. **주소 공간 전환** : 같은 프로세스 내에 존재하는 스레드가 아닐 경우에는 새 
스레드의 주소 공간으로의 전환이 요구됨

5. **실행 재개** : 저장된 PC 위치부터 실행 재개



---

### Context Switch Process for Kernel Threads

1. **State Saving**
When a context switch is triggered, the kernel saves the current thread's execution context to its Thread Control Block (TCB). This includes:

-   General purpose registers

-   Program Counter (PC)

-   Stack Pointer (SP)

-   Status registers

-   Floating point and SIMD registers (if used)

The kernel uses specialized assembly code to perform this operation efficiently.

2. **Scheduling**
The scheduler selects the next thread to run using its scheduling algorithm. This is typically done by the schedule() function in Linux which:

-   Examines ready queues

-   Applies priority rules

-   Selects the highest-priority runnable thread

-   The selection may be influenced by the TIF_NEED_RESCHED flag that gets set when a thread's time slice expires.

3. **State Restoration**
The kernel then loads the saved context from the selected thread's TCB, including:

-   All saved registers

-   Program counter

-   Stack pointer

-   Hardware execution state

4. **Address Space Switching** (Conditional)
If the new thread belongs to the same process as the previous thread, no address space switch is needed

If the new thread belongs to a different process, the kernel must switch the virtual memory mappings by changing page tables and flushing the TLB (Translation Lookaside Buffer)

This is why thread context switches within the same process are faster than process context switches.

5. **Execution Resumption**
The CPU resumes execution from the saved program counter location. The thread continues running as if it had never been interrupted.


--- 

### Key Points
Context switching between kernel threads requires kernel mode privileges

It's faster than process context switching but slower than user-level thread switching

Modern CPUs include optimizations to reduce context switch overhead

The hardware may avoid saving/loading unused parts of large register sets (like SIMD registers)

--- 

### Practice Questions
**What's the difference between context switching kernel threads and user threads?**

```text
Kernel thread switching requires kernel mode privileges and system calls

User thread switching happens entirely in user space without kernel involvement

User thread switching is generally faster
```

**Why is thread context switching faster than process context switching?**

```text
Threads within the same process share the same address space

No need to switch page tables or flush TLB when switching between threads in the same process
```

**What triggers a context switch between kernel threads?**

```text
Time slice expiration (preemptive multitasking)

Thread blocks waiting for I/O or synchronization

Thread voluntarily yields the CPU

Higher priority thread becomes runnable
```

**What specific data structures are involved in kernel thread context switching?**

```text
Thread Control Block (TCB)

Process Control Block (PCB) if switching between threads of different processes

CPU register state

In Linux: task_struct and thread_info structures
```
**How does CPU architecture impact context switching performance?**

```text
Register count and size affects how much state must be saved/restored

Some CPUs provide multiple register sets to reduce context switch overhead

Cache and TLB design impact the penalty of context switches
```
