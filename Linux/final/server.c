#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include "locker.h"

int MAX_CLIENTS;
struct Locker *lockers;

void saveDB(int locker_id) {
    FILE *db = fopen(DATABASE, "r+");
    if (db == NULL) {
        db = fopen(DATABASE, "w+");
        if (db == NULL) {
            perror("cannot access to db file");
            return;
        } else {
            fprintf(db, "%-10s %-10s %-15s %-15s %-s\n", "Locker No", "Available", "Password", "Content", "time");
            for (int i = 0; i < MAX_CLIENTS; i++) {
                fprintf(db, "%-10d %-10d %-15s %-15s %-ld\n", lockers[i].locker_id, lockers[i].in_use, lockers[i].password,
                        lockers[i].content, lockers[i].time);
            }
        }
        fclose(db);
        return;
    }

    fseek(db, 0, SEEK_SET);
    int line = 0;
    char buffer[BUFFER_SIZE];

    while (fgets(buffer, BUFFER_SIZE, db) != NULL) {
        if (line == (locker_id + 1)) {
            fseek(db, -strlen(buffer), SEEK_CUR);
            fprintf(db, "%-10d %-10d %-15s %-15s %-ld\n", lockers[locker_id].locker_id, lockers[locker_id].in_use, lockers[locker_id].password, lockers[locker_id].content, lockers[locker_id].time);
            break;
        }
        line++;
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
    for (int i = 0; i < MAX_CLIENTS; i++) {
        saveDB(i);
    }
}

void loadDB() {
    FILE *db = fopen(DATABASE, "r");
    if (db == NULL) {
        perror("cannot access to db file");
        initialize_lockers();
        return;
    }
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, db);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        fscanf(db, "%d %d %s %s %ld\n", &lockers[i].locker_id, &lockers[i].in_use, lockers[i].password, lockers[i].content, &lockers[i].time);
    }
    fclose(db);
}

void loadDBbyId(int locker_id) {
    FILE *db = fopen(DATABASE, "r");
    if (db == NULL) {
        perror("cannot access to db file");
        return;
    }
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, db); // 헤더 건너뛰기
    int line = 0;
    while (fgets(buffer, BUFFER_SIZE, db) != NULL) {
        if (line == locker_id) {
            sscanf(buffer, "%d %d %s %s %ld", &lockers[locker_id].locker_id, &lockers[locker_id].in_use, lockers[locker_id].password, lockers[locker_id].content, &lockers[locker_id].time);
            break;
        }
        line++;
    }
    fclose(db);
}

int checkPassword(int num, const char *password) {
    loadDBbyId(num);
    if (strcmp(password, lockers[num].password) == 0) {
        return 1;
    } else {
        return 0;
    }
}


void saveLogger(const char *message) {
    FILE *Logger = fopen(LoggerFile, "a");
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

void calculate_remaining_time(struct Locker *locker, char *buffer) {
    time_t current_time = time(NULL);
    int remaining_time = locker->duration - difftime(current_time, locker->time);
    if (remaining_time < 0) remaining_time = 0;
    snprintf(buffer, BUFFER_SIZE, "remain time : %d 초", remaining_time);
}

void handle_search(int client_socket) {
    char buffer[BUFFER_SIZE];
    int read_size;

    FILE *db = fopen(DATABASE, "r");
    if (db == NULL) {
        perror("cannot access to DB");
        saveLogger("cannot access to DB");
        return;
    }

    while (fgets(buffer, BUFFER_SIZE, db) != NULL) {
        int locker_id, in_use;
        sscanf(buffer, "%d %d", &locker_id, &in_use);
        if (locker_id >= 0 && locker_id < MAX_CLIENTS && (in_use == 0 || in_use == 1)) {
            snprintf(buffer, sizeof(buffer), "Locker No: %d | Available: %s\n", locker_id, in_use==0?"No":"Yes");
            send(client_socket, buffer, strlen(buffer), 0);
        }
    }
    strcpy(buffer, "end_of_list\n");
    send(client_socket, buffer, strlen(buffer), 0);
    fclose(db);

    while (1) {
        int locker_num;
        int received = recv(client_socket, &locker_num, sizeof(locker_num), 0);
        if (received <= 0) {
            saveLogger("locker_num received may be null...\n");
            break;
        }

        if (locker_num < 0 || locker_num >= MAX_CLIENTS) {
            strcpy(buffer, "wrong locker number.. please check again!\n");
            send(client_socket, buffer, strlen(buffer), 0);
            saveLogger("wrong locker number.. please check again!\n");
        } else {
            loadDBbyId(locker_num);
            if (lockers[locker_num].in_use == 0){
                snprintf(buffer, sizeof(buffer), "locker %d is empty\n", lockers[locker_num].locker_id);
                send(client_socket, buffer, strlen(buffer), 0);
            } else {
                char time_buffer[BUFFER_SIZE];
                calculate_remaining_time(&lockers[locker_num], time_buffer);
                snprintf(buffer, sizeof(buffer), "locker number: %d\navailability: %d\ncontents: %s\n",
                         lockers[locker_num].locker_id, lockers[locker_num].in_use, "secured data", time_buffer);
                send(client_socket, buffer, strlen(buffer), 0);

                char password[MAX_PASSWORD_SIZE];
                int password_received = recv(client_socket, password, sizeof(password), 0);
                if (password_received > 0) {
                    password[password_received] = '\0';
                    if (checkPassword(locker_num, password)) {
                        snprintf(buffer, sizeof(buffer), "locker number: %d | availability: %d | contents: %s\n",
                                 lockers[locker_num].locker_id, lockers[locker_num].in_use,
                                 lockers[locker_num].content);
                    } else {
                        strcpy(buffer, "wrong password!\n");
                    }
                    send(client_socket, buffer, strlen(buffer), 0);
                }
            }
        }
    }
}

void handle_reservation(int client_socket){

    char buffer[BUFFER_SIZE];
    int read_size;

    if ((read_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        int locker_id = atoi(buffer);

        int fd = open(DATABASE, O_RDWR);
        if (fd == -1){
            perror("Database access failed");
            saveLogger("Database access failed");
            close(client_socket);
            return;
        }

        /**
         * file lock initialize
         */
        struct flock lock;
        lock.l_type = F_WRLCK; // set write lock
        lock.l_whence = SEEK_SET;
        lock.l_start = locker_id * sizeof(struct Locker);
        lock.l_len = sizeof(struct Locker);
        lock.l_pid = getpid();

        if (fcntl(fd, F_SETLK, &lock) == -1){
            if (errno == EAGAIN){
                char* message = "this locker is on reservation...";
                saveLogger(message);
                send(client_socket, message, strlen(message), 0);
            } else {
                saveLogger("file lock fail");
            }
            close(fd);
            close(client_socket);
            return;
        }

        if (locker_id < 0 || locker_id >= MAX_CLIENTS) {
            char *message = "Invalid locker ID.\n";
            saveLogger(message);
            send(client_socket, message, strlen(message), 0);
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            close(fd);
            close(client_socket);
            return;
        }

        loadDBbyId(locker_id);

        if (lockers[locker_id].in_use == 0) {
            char *message = "Locker ID is available.\n";
            saveLogger(message);
            send(client_socket, message, strlen(message), 0);
        } else {
            char *message = "Locker ID is already in use.\n";
            saveLogger(message);
            send(client_socket, message, strlen(message), 0);
            lock.l_type = F_UNLCK;
            if (fcntl(fd, F_SETLK, &lock) == -1){
                saveLogger("file unlock failed");
            }
            close(fd);
            close(client_socket);
            return;
        }

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
                        continue;
                    } else {
                        char *message1 = "password equal";
                        saveLogger(message1);
                        send(client_socket, message1, strlen(message1), 0);

                        read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
                        if (read_size > 0) {
                            char content[BUFFER_SIZE];
                            sscanf(buffer, "%s", content);

                            saveLogger("content received from client server");

                            lockers[locker_id].in_use = 1;
                            strcpy(lockers[locker_id].password, password);
                            strcpy(lockers[locker_id].content, content);
                            saveDB(locker_id);

                            saveLogger("file updated to DB");

                            char *message = "Locker content saved.\n";
                            saveLogger(message);
                            send(client_socket, message, strlen(message), 0);
                            buffer[read_size] = '\0';
                            break;

                        }
                    }
                }
            }
        }
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1){
            saveLogger("file unlock fail");
        }
        close(fd);
    }
    close(client_socket);
    exit(0);
}

void handle_client(int client_socket) {
    int menu_choice;
    recv(client_socket, &menu_choice, sizeof(menu_choice), 0);

    switch (menu_choice) {
        case 1:
            handle_search(client_socket);
            break;
        case 2:
            handle_reservation(client_socket);
            break;
        case 3:
            // 결제 처리 로직 구현
            break;
        case 4:
            // 남은 시간 확인 로직 구현
            break;
        case 5:
            // 청구서 확인 로직 구현
            break;
        default:
            printf("잘못된 선택입니다\n");
            break;
    }

}

int main(int argc, char* argv[]) {
    if (argc != 2){
        fprintf(stderr, "Usage: %s <number_of_lockers>\n", argv[0]);
        return 1;
    }

    MAX_CLIENTS = atoi(argv[1]);
    if (MAX_CLIENTS <= 0){
        fprintf(stderr, "invalid number of locker, please type bigger than 0 again!\n");
        return 1;
    }

    lockers = malloc( sizeof(struct Locker) * MAX_CLIENTS);
    if (lockers == NULL){
        saveLogger("locker is null [memory error]");
        return 1;
    }

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

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

    loadDB();
    printf("server started.. waiting for any connections\n");

    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len))) {
        printf("Connection accepted\n");

        pid_t pid = fork();
        if (pid < 0){
            saveLogger("fork fail");
            continue;
        } else if (pid == 0)
        {
            close(server_socket);
            handle_client(client_socket);
            exit(0);
        } else {
            close(client_socket);
        }
    }

    if (client_socket < 0) {
        perror("Accept failed");
        return -1;
    }

    close(server_socket);
    return 0;
}
