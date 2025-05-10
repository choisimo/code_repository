#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>

int main(){

    int shm_fd = shm_open("/fork_test", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int));
    int *shared_cnt = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE , MAP_SHARED, shm_fd, 0);
    *share_cnt = 0;
}