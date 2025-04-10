#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "locker.h"

struct Locker lockers[MAX_CLIENTS];

void saveDB(){
    FILE *db = fopen(DATABASE, "w");
    if (db == NULL){
        perror("cannot access to db file");
        return;
    }
    fprintf(db, "%-10s %-10s %-15s\n", "Locker No", "Available", "Password");
    for (int i = 0; i < MAX_CLIENTS; i++){
        fprintf(db, "%-10d %-10d %-15s\n", lockers[i].locker_id, lockers[i].in_use, lockers[i].password);
    }
    fclose(db);
}

int initialize_lockers() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        lockers[i].locker_id = i;
        lockers[i].in_use = 0;
        strcpy(lockers[i].password, "");
        strcpy(lockers[i].content, "");  // content 필드는 초기화하되, DB에 저장하지 않음
    }
    saveDB();
}

void loadDB(){
    FILE *db = fopen(DATABASE, "r");
    if (db == NULL){
        perror("cannot access to db file");
        initialize_lockers();
        return;
    }
    for (int i = 0; i < MAX_CLIENTS; i++){
        fscanf(db, "%d %d %s\n", &lockers[i].locker_id, &lockers[i].in_use, lockers[i].password);
    }
    fclose(db);
}

void saveLogger(const char *message) {
    FILE *Logger = fopen(LoggerFile, "a");
    if (Logger == NULL) {
        perror("Logger file not exist");
        return;
    } else {
        fprintf(Logger, "%s\n", message);
        fclose(Logger);
    }
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int read_size;

    if ((read_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';

        int locker_id = atoi(buffer);

        if (locker_id < 0 || locker_id >= MAX_CLIENTS) {
            char *message = "Invalid locker ID.\n";
            saveLogger(message);
            send(client_socket, message, strlen(message), 0);
        } else {
            if (lockers[locker_id].in_use == 0) {
                char *message = "Locker ID is available.\n";
                saveLogger(message);
                send(client_socket, message, strlen(message), 0);

                while (1) {
                    read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
                    if (read_size > 0) {
                        buffer[read_size] = '\0';
                        char password[MAX_PASSWORD_SIZE];
                        sscanf(buffer, "%s", password);

                        char confirm_password[MAX_PASSWORD_SIZE];
                        read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
                        if (read_size > 0) {
                            buffer[read_size] = '\0';
                            sscanf(buffer, "%s", confirm_password);

                            if (strcmp(password, confirm_password) == 0) {
                                lockers[locker_id].in_use = 1;
                                strcpy(lockers[locker_id].password, password);
                                saveDB();
                                char *message = "Locker password set.\n";
                                saveLogger(message);
                                send(client_socket, message, strlen(message), 0);
                                break;
                            } else {
                                char *message = "Passwords do not match. Please try again.\n";
                                saveLogger(message);
                                send(client_socket, message, strlen(message), 0);
                            }
                        }
                    }
                }
            } else {
                char *message = "Locker ID is already in use.\n";
                saveLogger(message);
                send(client_socket, message, strlen(message), 0);
            }
        }
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    loadDB();

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Could not create socket");
        char* message = "could not create socket";
        saveLogger(message);
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return -1;
    }

    listen(server_socket, MAX_CLIENTS);

    printf("Waiting for incoming connections...\n");

    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len))) {
        printf("Connection accepted\n");
        handle_client(client_socket);
    }

    if (client_socket < 0) {
        perror("Accept failed");
        return -1;
    }

    close(server_socket);
    return 0;
}
