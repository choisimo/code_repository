#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"
/* 계좌 정보를 입력받아 데이터베이스 파일에 저장한다. */
int main(int argc, char *argv[])
{
   int fd;
   struct account acc;
   if (argc < 2) {
      fprintf(stderr, "사용법 : %s file\n", argv[0]);
      exit(1);
   }
   if ((fd = open(argv[1], O_WRONLY|O_CREAT|O_EXCL, 0640)) == -1) {
      perror(argv[1]);
      exit(2);
   }
   printf("%-9s %-8s %-4s\n", "ID", "Name", "Balance");
   while (scanf("%d %s %d", &acc.id, acc.name, &acc.balance) == 3) {
      lseek(fd, (acc.id - START_ID) * sizeof(acc), SEEK_SET);
      write(fd, (char *) &acc, sizeof(acc) );
   }
   close(fd);
   exit(0);
}

