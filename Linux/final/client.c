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
    char buffer[BUFFER_SIZE];
    while (1) {
        int received = recv(sock, buffer, sizeof(buffer), 0);
        if (received <= 0) {
            break;
        }
        buffer[received] = '\0';
        printf("%s", buffer);

        printf("want to see more specific info? (Y | N) \n");
        char choice = 'N';
        scanf("%c", &choice);
        if (choice == 'Y' || choice == 'y'){
            printf("enter the number of locker : ");
            int locker_num;
            scanf("%d", &locker_num);
            send(sock, &locker_num, sizeof(locker_num), 0);

            int detail_info = recv(sock, buffer, sizeof(buffer), 0);
            if (detail_info > 0) {
                buffer[detail_info] = '\0';
                printf("%s", buffer);
                printf("enter password to see secured content\n");
                printf("password : ");
                char password[MAX_PASSWORD_SIZE];
                scanf("%s", password);
                send(sock, password, strlen(password), 0);

                int content_received = recv(sock, buffer, BUFFER_SIZE, 0);
                if (content_received > 0){
                    buffer[content_received] = '\0';
                    printf("received data : \n %s\n", buffer);

                } else {
                    printf("failed to fetch secured content\n");
                    saveLogger("failed to fetch secured content\n");
                }
            } else {
                printf("error to fetch detail info\n");
                saveLogger("error to fetch detail info\n");
            }
        } else {
            printf("search menu off...\n");
            saveLogger("search menu off...\n");
            break;
        }
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