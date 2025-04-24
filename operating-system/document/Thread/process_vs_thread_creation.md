Process Creation vs Thread Creation: Memory States and Purpose
Process Creation with fork()
Duplicates the Process Control Block (PCB)

Purpose: Creates the administrative data structure for the new process

Memory Before: Single PCB exists in kernel memory

Memory After: Two separate PCBs exist, with child PCB containing copied values from parent (except unique values like PID)

Creates new virtual memory space (copies page tables)

Purpose: Establishes memory isolation between processes

Memory Before: Single set of page tables mapping virtual to physical addresses

Memory After: Two sets of page tables initially pointing to the same physical memory pages (marked as read-only for COW)

Duplicates all file descriptors and resources

Purpose: Allows child to access same resources as parent

Memory Before: Single set of file table entries

Memory After: Duplicated file table entries with shared file positions and status flags

Assigns new PID to child process

Purpose: Creates unique system-wide identifier for the new process

Memory Before: Only parent PID exists

Memory After: Child has unique PID stored in its PCB

Child gets copy of parent's memory (copy-on-write)

Purpose: Optimizes memory usage by deferring physical copying until necessary

Memory Before: Memory pages owned exclusively by parent

Memory After: Memory pages shared but marked as COW; physical copying occurs only when either process attempts to modify a page

Thread Creation
Creates Thread Control Block (TCB)

Purpose: Stores thread-specific execution context and state

Memory Before: Process has existing TCBs for current threads

Memory After: New TCB allocated in kernel memory (~20KB on 32-bit systems, ~40KB on 64-bit)

Allocates new stack for the thread

Purpose: Provides independent call stack for function execution

Memory Before: Process has existing thread stacks

Memory After: New stack space allocated within process address space (typically 256KB by default)

Sets up Thread Local Storage (TLS)

Purpose: Provides thread-private global variables

Memory Before: Existing TLS areas for other threads

Memory After: New TLS area allocated, allowing thread-specific "global" data without synchronization

Shares the process's address space (code, data, heap)

Purpose: Enables efficient data sharing between threads

Memory Before: Single address space with code, data and heap regions

Memory After: No change - all threads access the same memory regions

Shares file descriptors and most resources

Purpose: Allows unified resource management within the process

Memory Before: Process-wide resource tables

Memory After: No change - all process resources remain accessible to all threads

