#include <stdio.h>
#include <stdlib.h>
#include "account.h"

int main(int argc, char* argv[]){
    int account_num;
    printf("set the number of account: ");
    scanf("%d", &account_num);

    struct account* account1 = (struct account*) malloc(account_num * sizeof(struct account));
    if (account1 == NULL){
        printf("메모리 할당 오류");
        exit(0);
    }

    int i = 0;
    printf("Index   ID      Name    Balance\n");
    while(i < account_num)
    {
        printf("%d  ", i+1);
        scanf("%d %s %d", &account1[i].id, &account1[i].name, &account1[i].balance);
        i++;
    }

    while(1){
        int index = -1;
        printf("Account index to access: ");
        scanf("%d", &index);

        if (index < 1 || index > account_num)
        {
            printf("Index should be in 1 to %d\n", account_num);
            continue;
        }

        struct account *indexAcc = &account1[index - 1];
        printf("Index: %d ID:   %d   Name:%s     Balance: %d\n", index, indexAcc->id, indexAcc->name, indexAcc->balance);

        printf("Amount of money to withdraw: ");
        int amount = 0;
        scanf("%d", &amount);

        while(indexAcc->balance < amount){
            printf("should be smaller than %d\n", indexAcc->balance);
            printf("Amount of money to withdraw: ");
            scanf("%d", &amount);
        }
        indexAcc->balance -= amount;
        printf("Balance: %d\n", indexAcc->balance);
    }
    free(account1);
    return 0;

};
