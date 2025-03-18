
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "account.h"

struct account *addAccount(int id, char* name, int balance){
    struct account *newAccount = (struct account*) malloc(sizeof(struct account));
    if (newAccount == NULL)
    {
        printf("memory error");
        exit(1);
    } else {
        newAccount->id = id;
        strcpy(newAccount->name, name);
        newAccount->balance = balance;
        newAccount->next = NULL;
        return newAccount;
    }
}

int main(int argc, char* argv[]){

    int id;
    char name[MAX];
    int balance;
    struct account* head = NULL;
    struct account* tail = NULL;

    printf("ID    Name     Balance (ID=-1 to set the account information)\n");
    while(1){
        scanf("%d %s %d", &id, name, &balance);
        if (id == -1) {
            printf("Account Information\n");
            break;
        }
        struct account *newAccount = addAccount(id, name, balance);
        if (head == NULL){
            head = newAccount;
            tail = newAccount;
        } else {
            tail->next = newAccount;
            tail = newAccount;
        }
    }

    struct account *node = head;
    while(1){
        printf("ID: %d, Name: %s, Balance: %d\n", node->id, node->name, node->balance);
        char btn;
        printf("Press N or W (N: Next account, W: withdraw money): ");
        getchar();
        scanf("%c", &btn);
        if (btn == 'N'){
            node = node->next;
            if (node == NULL){
                node = head;
            }
        } else if (btn == 'W'){
            printf("Amount of money to withdraw: ");
            int amount = 0;
            scanf("%d", &amount);
            while (node->balance < amount){
                printf("should be smaller than %d\n", node->balance);
                printf("Amount of money to withdraw: ");
                scanf("%d", &amount);
            }
            node->balance -= amount;
            printf("ID: %d  Name: %s  Balance:%d\n", node->id, node->name, node->balance);
        }
    }

    struct account *temp;
    while(head){
        temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}

