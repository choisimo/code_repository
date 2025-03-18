#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"

int main(int argc, char *argv[]) {
    int fd, id;
    struct account record;
    char c;

    if (argc < 2) {
        fprintf(stderr, "사용법 : %s file\n", argv[0]);
        exit(1);
    }
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        exit(2);
    }

    do {
        printf("\nSearch for account ID: ");
        if (scanf("%d", &id) == 1) {
            lseek(fd, (id - START_ID) * sizeof(record), SEEK_SET);
            if ((read(fd, &record, sizeof(record)) > 0) && (record.id == id))
                printf("ID:%d\t Name:%s\t Balance:%d\n", record.id, record.name, record.balance);
            else
                printf("레코드 %d 없음\n", id);
        } else {
            printf("입력 오류");
        }
        printf("계속하겠습니까?(Y/N) ");
        scanf(" %c", &c);
    } while (c == 'Y' || c == 'y');

    close(fd);
    exit(0);
}
