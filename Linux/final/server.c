#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "locker.h"

struct Locker lockers[MAX_CLIENTS];

void initialize_lockers() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        lockers[i].locker_id = i;
        lockers[i].in_use = 0;
        strcpy(lockers[i].password, "");
        strcpy(lockers[i].content, "");
    }
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int read_size;

    // Receive locker id and password from client
    if ((read_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        int locker_id = atoi(buffer);
        
        // Check if locker id is valid
        if (locker_id < 0 || locker_id >= MAX_CLIENTS) {
            char *message = "Invalid locker ID.\n";
            send(client_socket, message, strlen(message), 0);
            close(client_socket);
            return;
        }

        // Receive password
        if ((read_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
            buffer[read_size] = '\0';
            if (strcmp(lockers[locker_id].password, buffer) == 0 || lockers[locker_id].in_use == 0) {
                lockers[locker_id].in_use = 1;
                strcpy(lockers[locker_id].password, buffer);

                // Receive content
                if ((read_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
                    buffer[read_size] = '\0';
                    strcpy(lockers[locker_id].content, buffer);
                    
                    char *message = "Locker content saved.\n";
                    send(client_socket, message, strlen(message), 0);
                }
            } else {
                char *message = "Incorrect password.\n";
                send(client_socket, message, strlen(message), 0);
            }
        }
    }

    close(client_socket);
}

void print_menu(){
    printf("┌─────────────────────────────────┐\n");
    printf("│         메뉴를 선택해주세요        │\n");
    printf("│─────────────────────────────────│\n");
    printf("│    1. 조회 │ 2. 예약 │ 3. 지불    │\n");
    printf("│─────────────────────────────────│\n");
    printf("│    4. 남은 시간 │ 5. 비용 확인     │\n");
    printf("└─────────────────────────────────┘\n");
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Initialize lockers
    initialize_lockers();

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Could not create socket");
        return 1;
    }

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen
    listen(server_socket, MAX_CLIENTS);

    printf("Waiting for incoming connections...\n");

    // Accept and incoming connection
    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len))) {
        printf("Connection accepted\n");
        handle_client(client_socket);
    }

    if (client_socket < 0) {
        perror("Accept failed");
        return 1;
    }

    close(server_socket);
    return 0;
}

