#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "locker.h"

struct Locker lockers[MAX_CLIENTS];
pthread_mutex_t locker_mutex;

void saveDB() {
    FILE *db = fopen(DATABASE, "w");
    if (db == NULL) {
        perror("cannot access to db file");
        return;
    }
    time_t curtime = time(NULL);
    struct tm tm = *localtime(&curtime);
    fprintf(db, "%-10s %-10s %-15s %-15s %-s\n", "Locker No", "Available", "Password", "Content", "time");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        fprintf(db, "%-10d %-10d %-15s %-15s %-s\n", lockers[i].locker_id, lockers[i].in_use, lockers[i].password, lockers[i].content, lockers[i].time);
    }
    fclose(db);
}

void initialize_lockers() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        lockers[i].locker_id = i;
        lockers[i].in_use = 0;
        strcpy(lockers[i].password, "");
        strcpy(lockers[i].content, "");
        lockers[i].time = 0;
    }
    saveDB();
}

void loadDB() {
    FILE *db = fopen(DATABASE, "r");
    if (db == NULL) {
        perror("cannot access to db file");
        initialize_lockers();
        return;
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        fscanf(db, "%d %d %s %s\n", &lockers[i].locker_id, &lockers[i].in_use, lockers[i].password, lockers[i].content);
    }
    fclose(db);
}

void saveLogger(const char *message) {
    FILE *Logger = fopen(LoggerFile, "a");
    if (Logger == NULL) {
        perror("Logger file not exist");
        return;
    } else {
        time_t curtime = time(NULL);
        struct tm tm = *localtime(&curtime);
        fprintf(Logger, "%lld %s\n", curtime, message);
        fclose(Logger);
    }
}

void* handle_client(void* client_socket_thread) {
    int client_socket = *(int*)client_socket_thread;
    free(client_socket_thread);
    char buffer[BUFFER_SIZE];
    int read_size;

    if ((read_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';

        int locker_id = atoi(buffer);

        pthread_mutex_lock(&locker_mutex);
        if (locker_id < 0 || locker_id >= MAX_CLIENTS) {
            char *message = "Invalid locker ID.\n";
            saveLogger(message);
            send(client_socket, message, strlen(message), 0);
        } else {
            if (lockers[locker_id].in_use == 0) {
                char *message = "Locker ID is available.\n";
                saveLogger(message);
                send(client_socket, message, strlen(message), 0);
            } else {
                char *message = "Locker ID is already in use.\n";
                saveLogger(message);
                send(client_socket, message, strlen(message), 0);
            }
        }
        pthread_mutex_unlock(&locker_mutex);

        while (1) {
            read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (read_size > 0) {
                buffer[read_size] = '\0';
                char password[MAX_PASSWORD_SIZE];
                sscanf(buffer, "%s", password);

                read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
                if (read_size > 0) {
                    buffer[read_size] = '\0';
                    char confirm_password[MAX_PASSWORD_SIZE];
                    sscanf(buffer, "%s", confirm_password);

                    if (strcmp(password, confirm_password) != 0) {
                        char *message = "password and confirm password not equal";
                        saveLogger(message);
                        send(client_socket, message, strlen(message), 0);
                    } else {
                        char *message1 = "password equal";
                        saveLogger(message1);
                        send(client_socket, message1, strlen(message1), 0);

                        read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
                        if (read_size > 0) {
                            char content[BUFFER_SIZE];
                            sscanf(buffer, "%s", content);

                            saveLogger("content received from client server");

                            pthread_mutex_lock(&locker_mutex);
                            lockers[locker_id].in_use = 1;
                            strcpy(lockers[locker_id].password, password);
                            strcpy(lockers[locker_id].content, content);
                            saveDB();
                            pthread_mutex_unlock(&locker_mutex);

                            saveLogger("file updated to DB");

                            char *message = "Locker content saved.\n";
                            saveLogger(message);
                            send(client_socket, message, strlen(message), 0);
                            break;
                        }
                    }
                }
            }
        }
    }
    close(client_socket);
    return NULL;
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
        saveLogger("Bind failed");
        return -1;
    }

    listen(server_socket, MAX_CLIENTS);

    printf("server started.. waiting for any connections\n");

    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len))) {
        printf("Connection accepted\n");

        pthread_t thread_id;
        int *client_sock_ptr = malloc(sizeof(int));
        *client_sock_ptr = client_socket;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)client_sock_ptr) < 0) {
            perror("could not create thread");
            free(client_sock_ptr);
            continue;
        }

        pthread_detach(thread_id);
    }

    if (client_socket < 0) {
        perror("Accept failed");
        return -1;
    }

    close(server_socket);
    return 0;
}
