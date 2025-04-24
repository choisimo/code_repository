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

## 문제 2 (Figure 3.22)
```c
#include 
#include 
#include 
int main()
{
pid_t pid;
/* fork a child process */
pid = fork();
if (pid 
#include 
#include 
int main()
{
pid_t pid, pid1;
/* fork a child process */
pid = fork();
if (pid 
#include 
#include 
#define SIZE 5
int nums[SIZE] = {0,1,2,3,4};
int main()
{
int i;
pid_t pid;
pid = fork();
if (pid == 0) {
for (i = 0; i  0) {
wait(NULL);
for (i = 0; i  0);
   }
   
   // 핸들러 설정
   signal(SIGCHLD, sigchld_handler);
   ```

3. **자식 프로세스를 이중 fork()하여 고아 프로세스로 만들기**:
   자식 프로세스가 다시 fork()를 호출하고 종료하면, 손자 프로세스는 init 프로세스(PID 1)에 입양되어 좀비가 되지 않습니다.

4. **자식 프로세스 종료 후 부모 프로세스가 계속 실행되어야 하는 장기 실행 프로그램에서는 SIGCHLD 시그널을 무시하도록 설정**:
   ```c
   signal(SIGCHLD, SIG_IGN);
   ```

좀비 프로세스 문제를 방지하기 위해서는 자식 프로세스를 생성하는 모든 프로그램이 자식 프로세스의 종료 상태를 적절히 수집하는 메커니즘을 구현하는 것이 중요합니다.
