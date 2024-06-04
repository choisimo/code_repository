#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "locker.h"

void print_menu(){
    printf(" please choose one menu following \n");
    printf(" 1. search \n");
    printf(" 2. reservation \n");
    printf(" 3. pay load \n");
    printf(" 4. remain time \n");
    printf(" 5. check current bill \n");
    printf("\n");
}

int get_menu_num(int menu_num){
    if(menu_num == 1){
        printf("조회 페이지 입니다.\n");
        return 1;
    } else if (menu_num == 2) {
        printf("예약 페이지 입니다.\n");
        return 2;
    } else if (menu_num == 3) {
        printf("지불 페이지 입니다.\n");
        return 3;
    } else if (menu_num == 4) {
        printf("잔여 시간 확인 페이지 입니다.\n");
        return 4;
    } else if (menu_num == 5) {
        printf("요금 확인 페이지 입니다.\n");
        return 5;
    } else {
        return 0;
    }
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    // socket create attempt
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error \n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        printf("IPv4 address not accepted \n");
        return -1;
    }

    // connect to server socket
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("error while connecting to server socket \n");
        return -1;
    }

    print_menu();
    int menu_choice = -1;
    printf("enter menu no : \n");
    scanf("%d", &menu_choice);
    getchar();

    //send(sock, buffer, strlen(buffer). 0);
    get_menu_num(menu_choice);

    printf("Enter locker ID: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    strcat(buffer, " ");
    //send(sock, buffer, strlen(buffer), 0);

    // Input password
    printf("Enter password: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    strcat(buffer, " ");
    //send(sock, buffer, strlen(buffer), 0);

    // Input content
    printf("Enter content to store in the locker: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    //send(sock, buffer strlen(buffer), 0);

    send(sock, buffer, strlen(buffer), 0);

    // Receive response from server
    int read_size = recv(sock, message, BUFFER_SIZE, 0);
    if (read_size > 0) {
        message[read_size] = '\0';
        printf("Server: %s\n", message);
    }

    close(sock);
    return 0;
}