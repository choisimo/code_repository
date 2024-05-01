//
// Created by csimo on 4/29/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "account.h"
int main(int argc, char *argv[])
{
    FILE* fp;
    int id;
    int num_of_account = 0;
    struct account record;
    char c;
    if (argc < 2) {
        fprintf(stderr, "»ç¿ë¹ý : %s file\n", argv[0]);
        exit(1);
    }
    if ((fp = fopen(argv[1], "rb")) == NULL) {
        perror(argv[1]);
        exit(2);
    }

    fread(&num_of_account, sizeof(int), 1, fp);
    printf("the number of accounts : %d\n", num_of_account);

    do {
        printf("\nSearch for account ID:");
        if (scanf("%d", &id) == 1) {
            fseek(fp, sizeof(int) + (id-START_ID)*sizeof(record), SEEK_SET);
            if (fread(&record, sizeof(record), 1, fp) > 0 && (record.id != 0))
                printf("ID:%d\t Name:%s\t balance:%d\n", record.id, record.name,
                       record.balance);
            else printf("record %d is not exist\n", id);
        } else printf("input error");
        printf("continue?(Y/N)");
        scanf(" %c", &c);
    } while (c=='Y');
    fclose(fp);
    exit(0);
}
