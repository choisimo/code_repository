//
// Created by csimo on 6/3/2024.
//

#ifndef CODE_REPOSITORY_LOCKER_H
#define CODE_REPOSITORY_LOCKER_H
#define MAX_PASSWORD_SIZE 128
#define BUFFER_SIZE 1024
#define PORT 8050
#define DATABASE "locker_database.txt"
#define LoggerFile "log.txt"
#define CLoggerFile "clog.txt"

struct Locker{
    int locker_id;
    char password[MAX_PASSWORD_SIZE];
    char content[BUFFER_SIZE];
    int in_use;
    time_t time;
    int duration;
    int draft;
};

#endif //CODE_REPOSITORY_LOCKER_H
