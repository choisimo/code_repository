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
    char c;
    struct account record;

    if (argc < 2) {
        fprintf(stderr, "»ç¿ë¹ý : %s file\n", argv[0]);
        exit(1);
    }
    if ((fp = fopen(argv[1], "rb+")) == NULL) {
        perror(argv[1]);
        exit(2);
    }

    fread(&num_of_account, sizeof(int), 1, fp);
    printf("the number of accounts : %d\n", num_of_account);

    do {
        printf("Account ID:");
        if (scanf("%d", &id) == 1) {
            fseek(fp, sizeof(int) + (long) (id-START_ID)*sizeof(record), SEEK_SET);
            if ((fread(&record, sizeof(record), 1, fp) > 0) && (record.id != 0)) {
                printf("ID:%8d\t Name:%4s\t Balance:%4d\n",
                       record.id, record.name, record.balance);
                printf("Amount of money to withdraw: ");
                scanf("%d", &record.balance);
                fseek(fp, (long) -sizeof(record), SEEK_CUR);
                fwrite(&record, sizeof(record), 1, fp);
            } else printf("record %d is not exist\n", id);
        } else printf("input error\n");
        printf("continue?(Y/N)");
        scanf(" %c",&c);
    } while (c == 'Y');
    fclose(fp);
    exit(0);
}

