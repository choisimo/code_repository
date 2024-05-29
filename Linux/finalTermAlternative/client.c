#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void *receive_messages(void *socket) {
    int sock = *((int *)socket);
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Server: %s\n", buffer);
    }

    return NULL;

}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    pthread_t tid;
    char message[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    pthread_create(&tid, NULL, receive_messages, (void *)&sock);

    while (1) {
        fgets(message, sizeof(message), stdin);
        if (send(sock, message, strlen(message), 0) < 0) {
            perror("Send failed");
            break;
        }
    }

    close(sock);
    return 0;
}

