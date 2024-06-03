#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"

#define MAX 24
#define START_ID 1401001

int main(int argc, char *argv[]) {
    int fd, id;
    char c;
    struct account record;
    int withdraw_amount;

    if (argc < 2) {
        fprintf(stderr, "사용법 : %s file\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror(argv[1]);
        exit(2);
    }

    do {
        printf("Account ID: ");
        if (scanf("%d", &id) == 1) {
            lseek(fd, (id - START_ID) * sizeof(record), SEEK_SET);
            if ((read(fd, &record, sizeof(record)) > 0) && (record.id == id)) {
                printf("ID:%8d\t NAME:%4s\t BALANCE:%4d\n", record.id, record.name, record.balance);
                printf("Amount of money to withdraw: ");
                if (scanf("%d", &withdraw_amount) == 1) {
                    if (withdraw_amount <= record.balance) {
                        record.balance -= withdraw_amount;
                        lseek(fd, -sizeof(record), SEEK_CUR);
                        write(fd, &record, sizeof(record));
                        printf("Balance: %d\n", record.balance);
                    } else {
                        printf("balance가 부족합니다.\n");
                    }
                } else {
                    printf("입력오류\n");
                    continue;
                }
            } else {
                printf("레코드 %d 없음\n", id);
            }
        } else {
            printf("입력오류\n");
            continue;
        }
        printf("계속하겠습니까?(Y/N) ");
        scanf(" %c", &c);

    } while (c == 'Y' || c == 'y');

    close(fd);
    exit(0);
}
