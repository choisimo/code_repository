#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"
#include <signal.h>

void intHandler();
char* filename;
/* Save account information */
int main(int argc, char *argv[])
{
   FILE *fp;
   struct account record;
   signal(SIGINT, intHandler);

   if (argc < 2) {
      fprintf(stderr, "사용법 : %s file\n", argv[0]);
      exit(1);
   }

   filename = argv[1];

   fp = fopen(argv[1], "wb");

   printf("%-9s %-8s %-4s\n", "ID", "Name", "Balance");
   while (scanf("%d %s %d", &record.id, record.name, &record.balance) == 3) {
	   if(record.id - START_ID < 0)
	   {
		   printf("should be larger than %d", START_ID);
		   continue;
	   }
      fseek(fp, (long) (record.id - START_ID) * sizeof(record), SEEK_SET);
      fwrite((char *) &record, sizeof(record), 1, fp);
   }
   fclose(fp);
   exit(0);
}

void intHandler(){
    execl("./ac_update", "ac_update",filename, (char*) NULL);
    exit(0);
}