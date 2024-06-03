//
// Created by csimo on 4/29/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "account.h"
int main(int argc, char *argv[])
{
    FILE* fp;
    struct account record;
    int num_of_account = 0;

    if (argc < 2) {
        fprintf(stderr, "»ç¿ë¹ý : %s file\n", argv[0]);
        exit(1);
    }
    fp = fopen(argv[1], "wb");
    if (fp == NULL){
        printf("can't open the file");
        exit(1);
    }

    printf("%-9s %-8s %-4s\n", "ID", "NAME", "balance");
    while (scanf("%d %s %d", &record.id, record.name, &record.balance)== 3) {
        fseek(fp, (record.id - START_ID) * sizeof(record) + sizeof(int), SEEK_SET);
        fwrite(&record, sizeof(record), 1, fp);
        num_of_account++;
    }
    fseek(fp, 0, SEEK_SET);
    fwrite(&num_of_account, sizeof(int), 1, fp);
    printf("the number of accounts : %d\n", num_of_account);
    fclose(fp);
    exit(0);
}
