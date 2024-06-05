#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
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
        printf("search page\n");
        printf("enter locker number your own\n");
        int locker_num = -1;
        scanf("%d", &locker_num);
        return locker_num;
    } else if (menu_num == 2) {
        printf("reservation page\n");
        return 2;
    } else if (menu_num == 3) {
        printf("payment page\n");
        return 3;
    } else if (menu_num == 4) {
        printf("check remain time page\n");
        return 4;
    } else if (menu_num == 5) {
        printf("check bill page\n");
        return 5;
    } else {
        return 0;
    }
}

struct Locker getLockerById(int sock, int locker_id) {
    send(sock, &locker_id, sizeof(locker_id), 0);
    struct Locker inLocker;
    recv(sock, &inLocker, sizeof(inLocker), 0);
    return inLocker;
}


void saveLogger(const char *message) {
    FILE *Logger = fopen(CLoggerFile, "a");
    if (Logger == NULL) {
        perror("Logger file not exist");
        return;
    } else {
        time_t curtime = time(NULL);
        struct tm tm = *localtime(&curtime);
        fprintf(Logger, "%ld %s\n", curtime, message);
        fclose(Logger);
    }
}

void handle_search(int sock) {
    int menu_value = 1;
    struct Locker locker1 = getLockerById(sock, menu_value);
    if (!locker1.locker_id){
        char* message = "not exist locker id... please check again\n";
        perror(message);
        saveLogger(message);
    } else {
        saveLogger("this id exists in DATABASE\n");
    }
}

void handle_searchById(int sock, int id){
    int menu_value = 1;
}

int handle_reservation(int sock) {
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    printf("Enter locker ID: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    send(sock, buffer, strlen(buffer), 0);

    int read_size = recv(sock, message, BUFFER_SIZE, 0);
    if (read_size > 0) {
        message[read_size] = '\0';
        printf("Server: %s\n", message);

        if (strcmp(message, "Locker ID is available.\n") == 0) {
            while(1){

                printf("Enter password: ");
                fgets(buffer, BUFFER_SIZE, stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                send(sock, buffer, strlen(buffer), 0);

                printf("Confirm password: ");
                fgets(buffer, BUFFER_SIZE, stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                send(sock, buffer, strlen(buffer), 0);

                read_size = recv(sock, message, BUFFER_SIZE, 0);
                if (read_size > 0) {
                    message[read_size] = '\0';
                    printf("Server: %s\n", message);

                    if (strcmp(message, "password and confirm password not equal") == 0) {
                        continue;
                    } else {
                        printf("Enter content to store in the locker: ");
                        fgets(buffer, BUFFER_SIZE, stdin);
                        buffer[strcspn(buffer, "\n")] = 0;
                        send(sock, buffer, strlen(buffer), 0);

                        read_size = recv(sock, message, BUFFER_SIZE, 0);
                        if (read_size > 0){
                            printf("Server: %s \n", message);
                            break;
                        } else {
                            perror("server error");
                            continue;
                        }
                    }
                }
            }
        } else if (strcmp(message, "Locker ID is already in use.\n") == 0) {
            close(sock);
            execl("./client", "client", (char *)NULL);
            return 0;
        }
    }
}


    /**
====================================================================================
                               소켓 연결 시작
====================================================================================
 * */
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

    /**
====================================================================================
                                   소켓 연결 끝
====================================================================================
     * */


    print_menu();
    printf("choose one menu num... : \n");
    int menu_choice;
    scanf("%d", &menu_choice);
    getchar();

    send(sock, &menu_choice, sizeof(menu_choice), 0);

    if (menu_choice == 1){
        handle_search(sock);
    } else if (menu_choice == 2){
        handle_reservation(sock);
    } else if (menu_choice == 3){

    } else if (menu_choice == 4){

    } else if (menu_choice == 5){

    } else {

    }
    close(sock);
    return 0;
}