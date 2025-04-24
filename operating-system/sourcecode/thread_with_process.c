#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();   // 부모는 자식의 PID, 자식은 0을 가진다.
    // 자식 프로세스 일 경우에는
    if (pid == 0) {
        fork(); 
        thread create();
    }
    fork();
}

/**
                 PROCESS vs THREAD CREATION
                 ==========================

PROCESS CREATION (fork())                THREAD CREATION
----------------------                   ---------------

  +--------+                              +--------+
  | Parent |                              | Process|
  | Process|                              |        |
  +--------+                              +---+----+
      |                                       |
      | fork()                                | thread_create()
      |                                       |
      v                                       v
 +-----------+                         +-------------+
 | KERNEL    |                         | KERNEL      |
 +-----------+                         +-------------+
      |                                       |
      | 1. Copy PCB                           | 1. Create TCB
      | 2. Copy page tables                   | 2. Allocate stack
      | 3. Copy file descriptors              | 3. Setup TLS
      | 4. Assign new PID                     | 4. Share address space
      |                                       |
      v                                       v
  +---------+    +---------+            +--------+
  | Parent  |    | Child   |            | Process|==Thread1
  | Process |    | Process |            |        |==Thread2
  +---------+    +---------+            +--------+
  |  Memory |    |  Memory |            |   Shared Memory  |
  +---------+    +---------+            +----------------+
*/