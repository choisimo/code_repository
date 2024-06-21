#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"

int main(int argc, char *argv[])
{
    int fd, id;
    char c;
    struct account acc;
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
            lseek(fd, (long)(id-START_ID)*sizeof(acc), SEEK_SET);
            if ((read(fd, &acc, sizeof(acc)) > 0) && (acc.id != 0)) {
                printf("ID:%8d\t Name:%4s\t Balance:%4d\n",
                       acc.id, acc.name, acc.balance);
                printf("Amount of money to withdraw: ");
                int withdraw_amount;
                scanf("%d", &withdraw_amount);
                // 출금 금액이 양수이고, 잔액보다 같거나 작을 때만 처리
                if (withdraw_amount > 0 && withdraw_amount <= acc.balance) {
                    acc.balance -= withdraw_amount;
                    lseek(fd, (long)-sizeof(acc), SEEK_CUR);
                    write(fd, &acc, sizeof(acc));
                } else if (withdraw_amount <= 0) {
                    acc.balance -= withdraw_amount;
                     lseek(fd, (long)-sizeof(acc), SEEK_CUR);
                    write(fd, &acc, sizeof(acc));
                } else {
                    printf("should be smaller than %d\n Balance: %d\n", acc.balance, acc.balance);
                }
            } else {
                printf("레코드 %d 없음\n", id);
            }
        } else {
            printf("입력오류\n");
            while (getchar() != '\n'); // 입력 버퍼 비우기
        }
        printf("계속하겠습니까?(Y/N) ");
        scanf(" %c", &c);
        while (getchar() != '\n'); // 입력 버퍼 비우기
    } while (c == 'Y' || c == 'y');
    close(fd);
    exit(0);
}
