#include <stdio.h>
#include <stdlib.h>
#include "account.h"

int main(int argc, char* argv[])
{
    struct account *record;
    int n;
    printf("set the number of account: ");
    scanf("%d", &n);
    record = (struct account)malloc(n * sizeof(struct account));
    printf("%-9s %-9s %-8s %-4s\n", "Index", "ID", "Name", "Balance");
    for (int i = 0; i < n; i++){
        printf("%d", i + 1);
        scanf("%d %s %d", record[i].id, record[i].name. &record[i].balance);
    }
    for (int i = 0; i < n; i++){
        printf("%d %s %d\n", record[i].id, record[i].name, &record[i].balance);
    }
    free(record);
    exit(0);
}
