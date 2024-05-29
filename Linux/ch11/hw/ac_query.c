#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"
/* Search account information */
int main(int argc, char *argv[])
{
   int id;
   FILE *fp;
   struct account record;
   char c;
   if (argc < 2) {
      fprintf(stderr, "사용법 : %s file\n", argv[0]);
      exit(1);
   }
   if ((fp = fopen(argv[1], "rb")) == NULL)
   {
      perror(argv[1]);
      exit(2);
   }
   do {
      printf("\nSearch for account ID:");
      if (scanf("%d", &id) == 1)
      {
         fseek(fp, (long) (id-START_ID)*sizeof(record), SEEK_SET);
         if ((fread(&record, sizeof(record), 1, fp) > 0) && (record.id != 0))
         {
            printf("ID:%d\t Name:%s\t Balance:%d\n", record.id, record.name,
                    record.balance);
         }
         else
         {
        	 printf("레코드 %d 없음\n", id);
         }
      }
      else
    	  printf("입력 오류");
      printf("계속하겠습니까?(Y/N)");
      scanf(" %c", &c);
   } while (c=='Y');
   fclose(fp);
   exit(0);
}
