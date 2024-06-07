#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include "locker.h"
#include "socket.h"

int MAX_CLIENTS;
struct Locker *lockers;
int server_socket;
struct clientInfo *clients;
int client_count = 0;

void saveDB(int locker_id);
void saveLogger(const char* message);

void add_client(int socket, int locker_id){
    struct clientInfo *newClients = (struct clientInfo *)malloc(sizeof(struct clientInfo) * (client_count + 1));
    if (newClients == NULL){
        perror("memory allocate error");
        saveLogger("memory allocate error");
        return;
    }
    if (clients != NULL){
        memcpy(newClients, clients, sizeof(struct clientInfo) * client_count);
        free(clients);
    }
    clients = newClients;
    clients[client_count].socket = socket;
    clients[client_count].locker_id = locker_id;
    client_count++;

    char logMessage[BUFFER_SIZE];
    sprintf(logMessage, "current client_count : %d", client_count);
    saveLogger("new client socker add\n");
    saveLogger(logMessage);
}

void remove_client(int socket){
    for (int i = 0; i < client_count; i++){
        if (clients[i].socket == socket){
            for (int j = 0; j < client_count - 1; j++){
                clients[j] = clients[j + 1];
            }
            client_count--;
            struct clientInfo *newClients = (struct clientInfo *)malloc(sizeof(struct clientInfo) * (client_count + 1));
            if (newClients == NULL){
                perror("memory allocate error");
                saveLogger("memory allocate error");
                return;
            }
            if (client_count > 0){
                memcpy(newClients, clients, sizeof(struct clientInfo)* client_count);
            }
            free(clients);
            clients = newClients;
            break;
        }
    }
}


void handle_exit(int sig) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket == -1 && lockers[clients[i].locker_id].draft == 1) {
            lockers[clients[i].locker_id].draft = 0;
            saveDB(clients[i].locker_id);
        }
    }
    close(server_socket);
    printf("server shutdown..\n");
    exit(0);
}

void signal_handler(){
    struct sigaction signal;
    signal.sa_handler = handle_exit;
    sigemptyset(&signal.sa_mask);
    signal.sa_flags = 0;
    if (sigaction(SIGINT, &signal, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}



void saveDB(int locker_id) {
    FILE *db = fopen(DATABASE, "r+");
    if (db == NULL) {
        db = fopen(DATABASE, "w+");
        if (db == NULL) {
            perror("cannot access to db file");
            return;
        } else {
            fprintf(db, "%-10s %-10s %-10s %-15s %-15s %-10s %-s\n", "Locker No", "Available", "Draft", "Password", "Content", "time", "duration");
            for (int i = 0; i < MAX_CLIENTS; i++) {
                char message[BUFFER_SIZE];
                sprintf(message, "Locker %d is saving now", lockers[i].locker_id);
                saveLogger(message);
                fprintf(db, "%-10d %-10d %-10d %-15s %-15s %-10lld %-d\n", lockers[i].locker_id, lockers[i].in_use, lockers[i].draft, lockers[i].password, lockers[i].content, lockers[i].time, lockers[i].duration);
            }
        }
        fclose(db);
        return;
    }

    fseek(db, 0, SEEK_SET);
    int line = 0;
    char buffer[BUFFER_SIZE];
    long position = 0;

    while (fgets(buffer, BUFFER_SIZE, db) != NULL) {
        if (line == (locker_id + 1)) {
            position = ftell(db);
            fseek(db, position - strlen(buffer), SEEK_SET);
            fprintf(db, "%-10d %-10d %-10d %-15s %-15s %-10lld %-d\n", lockers[locker_id].locker_id, lockers[locker_id].in_use, lockers[locker_id].draft, lockers[locker_id].password, lockers[locker_id].content, lockers[locker_id].time, lockers[locker_id].duration);
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
        lockers[i].draft = 0;
        strcpy(lockers[i].password, "");
        strcpy(lockers[i].content, "");
        lockers[i].time = 0;
        lockers[i].duration = 0;
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
        if (fgets(buffer, BUFFER_SIZE, db) != NULL){
            fscanf(db, "%d %d %d %s %s %lld %d\n", &lockers[i].locker_id, &lockers[i].in_use, &lockers[i].draft, lockers[i].password, lockers[i].content, &lockers[i].time, &lockers[i].duration);
        }
    }
    fclose(db);
}

void updateLocker(int locker_id) {
    loadDB();
    saveDB(locker_id);
}

void loadDBbyId(int locker_id) {
    FILE *db = fopen(DATABASE, "r");
    if (db == NULL) {
        perror("cannot access to db file");
        return;
    }
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, db); // skip header info
    int line = 0;
    while (fgets(buffer, BUFFER_SIZE, db) != NULL) {
        if (line == locker_id) {
            sscanf(buffer, "%d %d %d %s %s %lld %d", &lockers[locker_id].locker_id, &lockers[locker_id].in_use, &lockers[locker_id].draft, lockers[locker_id].password, lockers[locker_id].content, &lockers[locker_id].time, &lockers[locker_id].duration);
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

// 0 : reg_time, 1 : end_time, 2: reg + end time
void calculate_remaining_time(struct Locker *locker, char *buffer, int type) {
    time_t current_time = time(NULL);
    int remaining_time = locker->duration - difftime(current_time, locker->time);
    if (remaining_time < 0) {
        remaining_time = 0;
    }
    time_t reg_time = locker->time;
    time_t end_time = locker->time + locker->duration;

    char logBuffer[BUFFER_SIZE];
    sprintf(logBuffer, "regtime : %ld, end_time : %ld", reg_time, end_time);
    saveLogger(logBuffer);

    struct tm reg_time_tm;
    struct tm end_time_tm;

    localtime_r(&reg_time, &reg_time_tm);
    localtime_r(&end_time, &end_time_tm);

    sprintf(logBuffer, "regtime_tm : %ld, endtime_tm : %ld", (long)&reg_time_tm, (long)&end_time_tm);
    saveLogger(logBuffer);

    char reg_time_str[BUFFER_SIZE];
    char end_time_str[BUFFER_SIZE];

    switch(type) {
        case 0:
            strftime(buffer, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S (KST)", &reg_time_tm);
            break;
        case 1:
            strftime(buffer, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S (KST)", &end_time_tm);
            break;
        case 2:
            strftime(reg_time_str, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S (KST)", &reg_time_tm);
            strftime(end_time_str, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S (KST)", &end_time_tm);
            snprintf(buffer, BUFFER_SIZE, "Registered at: %s | Expires at: %s", reg_time_str, end_time_str);
            break;
        default:
            snprintf(buffer, BUFFER_SIZE, "Invalid time type");
            break;
    }
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

    fgets(buffer, BUFFER_SIZE, db); // skip header
    int line = 0;
    while (fgets(buffer, BUFFER_SIZE, db) != NULL) {
        int locker_id, in_use, draft;
        sscanf(buffer, "%d %d %d", &locker_id, &in_use, &draft);
        if (line >= 0 && line < MAX_CLIENTS) {
            lockers[line].locker_id = line;
            lockers[line].in_use = in_use;
            lockers[line].draft = draft;

            char message[BUFFER_SIZE];
            snprintf(message, sizeof(message), "Locker No: %d | Available: %s\n", line, (in_use == 0 && draft == 0) ? "Yes" : "No");
            send(client_socket, message, strlen(message), 0);
        }
        line++;
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

            if (lockers[locker_num].in_use == 0) {
                snprintf(buffer, sizeof(buffer), "locker %d is empty\n", lockers[locker_num].locker_id);
                send(client_socket, buffer, strlen(buffer), 0);
            } else {
                char reg_time_buffer[BUFFER_SIZE], dual_time_buffer[BUFFER_SIZE];
                calculate_remaining_time(&lockers[locker_num], reg_time_buffer, 0);
                snprintf(buffer, sizeof(buffer), "locker number: %d | availability: %s | contents: %s \n registered at : %s",
                         lockers[locker_num].locker_id, (lockers[locker_num].in_use == 0) || (lockers[locker_num].draft == 0) ? "True":"False", "secured data", reg_time_buffer);
                send(client_socket, buffer, strlen(buffer), 0);

                char password[MAX_PASSWORD_SIZE];
                int password_received = recv(client_socket, password, sizeof(password), 0);
                if (password_received > 0) {
                    password[password_received] = '\0';
                    if (checkPassword(locker_num, password)) {
                        calculate_remaining_time(&lockers[locker_num], dual_time_buffer, 2);
                        snprintf(buffer, sizeof(buffer), "locker number: %d | availability: %s | contents: %s\n %s",
                                 lockers[locker_num].locker_id, (lockers[locker_num].in_use == 0) || (lockers[locker_num].draft == 0) ? "True":"False",
                                 lockers[locker_num].content, dual_time_buffer);
                    } else {
                        strcpy(buffer, "wrong password!\n");
                    }
                    send(client_socket, buffer, strlen(buffer), 0);
                }
            }
        }
    }
}

/*
void handle_search(int client_socket) {
    char buffer[BUFFER_SIZE];
    int read_size;

    FILE *db = fopen(DATABASE, "r");
    if (db == NULL) {
        perror("cannot access to DB");
        saveLogger("cannot access to DB");
        return;
    }

    fgets(buffer, BUFFER_SIZE, db); // skip header
    while (fgets(buffer, BUFFER_SIZE, db) != NULL) {
        int locker_id, in_use, draft;
        sscanf(buffer, "%d %d %d", &locker_id, &in_use, &draft);
        if (locker_id >= 0 && locker_id < MAX_CLIENTS) {
            snprintf(buffer, sizeof(buffer), "Locker No: %d | Available: %s\n", locker_id, (in_use == 0 && draft == 0) ? "Yes" : "No");
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

            if (lockers[locker_num].in_use == 0 && lockers[locker_num].draft == 0) {
                snprintf(buffer, sizeof(buffer), "locker %d is empty\n", lockers[locker_num].locker_id);
                send(client_socket, buffer, strlen(buffer), 0);
            }
            else if(lockers[locker_num].in_use == 0 && lockers[locker_num].draft == 1)
            {
                snprintf(buffer, sizeof(buffer), "locker %d is on reservation\n", lockers[locker_num].locker_id);
                send(client_socket, buffer, strlen(buffer), 0);
            }
            else {
                    char reg_time_buffer[BUFFER_SIZE], dual_time_buffer[BUFFER_SIZE];
                    calculate_remaining_time(&lockers[locker_num], reg_time_buffer, 0);
                    snprintf(buffer, sizeof(buffer), "locker number: %d | availability: %s | contents: %s \n registered at : %s",
                             lockers[locker_num].locker_id, (lockers[locker_num].in_use == 0) || (lockers[locker_num].draft == 0) ? "True":"False", "secured data", reg_time_buffer);
                    send(client_socket, buffer, strlen(buffer), 0);

                    char password[MAX_PASSWORD_SIZE];
                    int password_received = recv(client_socket, password, sizeof(password), 0);
                    if (password_received > 0) {
                        password[password_received] = '\0';
                        if (checkPassword(locker_num, password)) {
                            calculate_remaining_time(&lockers[locker_num], dual_time_buffer, 2);
                            snprintf(buffer, sizeof(buffer), "locker number: %d | availability: %s | contents: %s\n %s",
                                     lockers[locker_num].locker_id, (lockers[locker_num].in_use == 0) || (lockers[locker_num].draft == 0) ? "True":"False",
                                     lockers[locker_num].content, dual_time_buffer);
                        } else {
                            strcpy(buffer, "wrong password!\n");
                        }
                        send(client_socket, buffer, strlen(buffer), 0);
                    }
                }
            }
        }
    }
*/

void handle_checkout(int client_socket) {
    char buffer[BUFFER_SIZE];
    int read_size;
    int locker_id = -1;

    while ((read_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        locker_id = atoi(buffer);

        char log_message[read_size];
        sprintf(log_message,"check out locker number : %d", locker_id);
        saveLogger(log_message);

        int fd = open(DATABASE, O_RDWR);
        if (fd == -1) {
            perror("DATABASE access failed");
            saveLogger("DATABASE access failed");
            close(client_socket);
            return;
        }

        if (locker_id < 0 || locker_id > MAX_CLIENTS) {
            char *error_message = "wrong locker_id received...";
            perror(error_message);
            saveLogger(error_message);
            send(client_socket, error_message, strlen(error_message), 0);
            break;
        }

        loadDBbyId(locker_id);

        while (1) {
            if (lockers[locker_id].in_use == 0){
                char *error_message = "Locker is already empty.";
                perror(error_message);
                saveLogger(error_message);
                send(client_socket, error_message, strlen(error_message), 0);
                break;
            }

            send(client_socket, "", BUFFER_SIZE, 0);

            while(1){
            int clientRecv;
            if ((clientRecv = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
                buffer[clientRecv] = '\0';
                char password[MAX_PASSWORD_SIZE];
                sscanf(buffer, "%s", password);
                if (strstr(password, lockers[locker_id].password) != NULL) {
                    char *message = "password correct. Really want to checkout? (Y/N) : \n";
                    send(client_socket, message, strlen(message), 0);

                    clientRecv = recv(client_socket, buffer, BUFFER_SIZE, 0);
                    if (clientRecv > 0) {
                        buffer[clientRecv] = '\0';
                        if (strcmp(buffer, "Y") == 0 || strcmp(buffer, "y") == 0) {
                            lockers[locker_id].in_use = 0;
                            lockers[locker_id].draft = 0;
                            strcpy(lockers[locker_id].password, "");
                            strcpy(lockers[locker_id].content, "");
                            lockers[locker_id].time = 0;
                            lockers[locker_id].duration = 0;
                            saveDB(locker_id);

                            char logMessage[BUFFER_SIZE];
                            sprintf(logMessage, "Locker %d has been checked out and is now empty.",
                                    lockers[locker_id].locker_id);
                            saveLogger(logMessage);

                            message = "Locker content deleted successfully.\n";
                            send(client_socket, message, strlen(message), 0);
                        } else {
                            char *message = "Checkout canceled.\n";
                            send(client_socket, message, strlen(message), 0);
                        }
                    } else {
                        perror("failed to fetch confirmation info");
                        saveLogger("failed to fetch confirmation info");
                    }
                } else {
                    char *message = "wrong password!";
                    perror(message);
                    saveLogger(message);
                    send(client_socket, message, strlen(message), 0);
                }
            } else {
                saveLogger("failed to fetch password info");
                }
            }
        }
    }
}

int handle_reservation(int client_socket) {
    char buffer[BUFFER_SIZE];
    int read_size;
    int locker_id = -1;

    while ((read_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        locker_id = atoi(buffer);

        int fd = open(DATABASE, O_RDWR);
        if (fd == -1) {
            perror("Database access failed");
            saveLogger("Database access failed");
            close(client_socket);
            return -1;
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

        if (fcntl(fd, F_SETLK, &lock) == -1) {
            if (errno == EAGAIN) {
                char* message = "This locker is on reservation by another user.";
                saveLogger(message);
                send(client_socket, message, strlen(message), 0);
            } else {
                saveLogger("file lock fail");
            }
            close(fd);
            continue;
        }

        if (locker_id < 0 || locker_id >= MAX_CLIENTS) {
            char *message = "Invalid locker ID.\n";
            saveLogger(message);
            send(client_socket, message, strlen(message), 0);
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            close(fd);
            continue;
        }

        loadDBbyId(locker_id);

        if (lockers[locker_id].in_use == 0 && lockers[locker_id].draft == 0) {
            saveLogger("locker is not in use and not in draft status\n");
            char *message = "Locker ID is available.\n";
            saveLogger(message);
            send(client_socket, message, strlen(message), 0);

            lockers[locker_id].draft = 1;
            saveDB(locker_id);

            while (1) {
                read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
                // reset draft if connection failed
                if(read_size <= 0){
                    lockers[locker_id].draft = 0;
                    saveDB(locker_id);
                    return -1;
                }

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
                                buffer[read_size] = '\0';
                                char content[BUFFER_SIZE];
                                sscanf(buffer, "%s", content);

                                saveLogger("content received from client server");

                                // 사용자 이용 시간 지정
                                read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
                                if (read_size > 0){
                                    buffer[read_size] = '\0';
                                    int duration = atoi(buffer);

                                    lockers[locker_id].in_use = 1;
                                    strcpy(lockers[locker_id].password, password);
                                    strcpy(lockers[locker_id].content, content);
                                    lockers[locker_id].time = time(NULL);
                                    lockers[locker_id].duration = duration * 3600;
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
            }
        } else {
            char *message = "Locker ID is already in use.\n";
            saveLogger(message);
            send(client_socket, message, strlen(message), 0);
            lock.l_type = F_UNLCK;
            if (fcntl(fd, F_SETLK, &lock) == -1) {
                saveLogger("file unlock failed");
            }
            close(fd);
            continue;
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            saveLogger("file unlock fail");
        }
        close(fd);
    }

    if(locker_id >= 0){
        lockers[locker_id].draft = 0;
        saveDB(locker_id);
    }

    close(client_socket);
    return locker_id;
}

void handle_client(int client_socket) {
    int menu_choice;
    recv(client_socket, &menu_choice, sizeof(menu_choice), 0);
    int locker_id = -1;

    switch (menu_choice) {
        case 1:
            handle_search(client_socket);
            break;
        case 2:
            locker_id = handle_reservation(client_socket);
            if (locker_id >= 0){
                add_client(client_socket, locker_id);
            }
            break;
        case 3:
            handle_checkout(client_socket);
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
    remove_client(client_socket);
    close(client_socket);
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

    signal_handler();
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
