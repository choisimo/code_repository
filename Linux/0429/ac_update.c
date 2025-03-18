#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"
#define MAX 24
#define START_ID 1401001
int main(int argc, char *argv[]) {
    int fd, id;
    char c;
    struct student record;
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
            // Ensure the ID is within a valid range
            if (id < START_ID) {
                fprintf(stderr, "Invalid account ID\n");
                continue;
            }

            // Calculate the file offset
            off_t offset = sizeof(int) + (long)(id - START_ID) * sizeof(record);
            off_t pos = lseek(fd, offset, SEEK_SET);
            if (pos == (off_t)-1) {
                perror("lseek");
                close(fd);
                exit(3);
            }

            // Read the record
            if ((read(fd, &record, sizeof(record)) > 0) && (record.id == id)) {
                printf("ID:%8d\t NAME:%4s\t BALANCE:%4d\n", record.id, record.name, record.balance);
                printf("Amount of money to withdraw: ");
                if (scanf("%d", &withdraw_amount) == 1) {
                    if (withdraw_amount <= record.balance) {
                        record.balance -= withdraw_amount;

                        // Reposition the file pointer to the start of the record for writing
                        pos = lseek(fd, (long)-sizeof(record), SEEK_CUR);
                        if (pos == (off_t)-1) {
                            perror("lseek");
                            close(fd);
                            exit(4);
                        }

                        // Write the updated record back to the file
                        if (write(fd, &record, sizeof(record)) != sizeof(record)) {
                            perror("write");
                            close(fd);
                            exit(5);
                        }
                        printf("Balance: %d\n", record.balance);
                    } else {
                        printf("balance가 부족합니다.\n");
                    }
                } else {
                    printf("입력오류\n");
                    // Clear the input buffer to avoid infinite loop in case of input error
                    while ((c = getchar()) != '\n' && c != EOF);
                    continue;
                }
            } else {
                printf("레코드 %d 없음\n", id);
            }
        } else {
            printf("입력오류\n");
            // Clear the input buffer to avoid infinite loop in case of input error
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        printf("계속하겠습니까?(Y/N) ");
        scanf(" %c", &c);

    } while (c == 'Y' || c == 'y');

    close(fd);
    return 0;
}
