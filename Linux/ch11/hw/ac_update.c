#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"
#include <signal.h>
int pid, limit_time = 20;
char* filename = NULL;

void alarmHandler();
void printRemainTime(int sig);

/* transfer money */
int main(int argc, char *argv[])
{
    printf("\nTransfer money in 20 seconds\n");

    int child, status;
    signal(SIGALRM, alarmHandler);
    signal(SIGUSR1, printRemainTime);

    pid = getpid();

    int id;
    FILE *fp;
    int cur_balance, money;
    char c;
    struct account record;
    if (argc < 2) {
        fprintf(stderr, "사용법 : %s file\n", argv[0]);
        exit(1);
    }
    if ((fp = fopen(argv[1], "rb+")) == NULL)
    {
        perror(argv[1]);
        exit(2);
    }

    filename = argv[1];

    alarm(5);

    do {
        printf("Account ID: ");
        if (scanf("%d", &id) == 1) {
            fseek(fp, (long) (id-START_ID)*sizeof(record), SEEK_SET);
            if ((fread(&record, sizeof(record), 1, fp) > 0) && (record.id != 0)) {
                printf("ID:%8d\t Name:%4s\t Balance:%4d\n",
                       record.id, record.name, record.balance);
                printf("Amount of money to withdraw: ");
                scanf("%d", &money);
                cur_balance = record.balance - money;
                if (cur_balance < 0)
                {
                    printf("should be smaller than %d\n", record.balance);
                    printf("Balance: %d\n", record.balance);
                }
                else
                {
                    record.balance = cur_balance;
                    fseek(fp, (long) -sizeof(record), SEEK_CUR);
                    fwrite(&record, sizeof(record), 1, fp);
                    printf("Balance: %d\n", record.balance);
                }
            } else printf("레코드 %d 없음\n", id);
        } else printf("입력오류\n");
        printf("계속하겠습니까?(Y/N)");
        scanf(" %c",&c);
    } while (c == 'Y');
    fclose(fp);
    printf("end\n");
    exit(0);
}

void alarmHandler(){
    if (limit_time <= 0){
    fflush(stdout);
    execl("./ac_query", "ac_query", filename, (char *)NULL);
    exit(0);
    } else {
        printRemainTime(SIGUSR1);
    }
}

void printRemainTime(int sig){
    limit_time -= 5;
    printf("%d sec\n", limit_time);
    if (limit_time > 0) {
        printf("\n");
        alarm(5);
    } else {
        printf("\n");
        printf("20sec alarm\n");
        alarmHandler();
    }
}

