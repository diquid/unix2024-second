#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>


#define LOCK_SUFFIX ".lck"

static int SUCCESSFUL_LOCKS_COUNT = 0;
static pid_t PID;


void log_error(const char *error_message) {
    int fd = open("errors.log", O_WRONLY | O_APPEND | O_CREAT, 0666);
    if (fd != -1) {
        time_t raw_time;
        struct tm *time_info;
        char time_str[80];

        time(&raw_time);
        time_info = localtime(&raw_time);

        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);

        dprintf(fd, "[%s] Error: %s\n", time_str, error_message);
        close(fd);
    }
}


void killer(int signal) {
    int fd = open("statistics", O_WRONLY | O_APPEND | O_CREAT, 0666);
    if (fd != -1) {
        dprintf(fd, "Process %d locked file %d times\n", PID, SUCCESSFUL_LOCKS_COUNT);
        close(fd);
    }

    exit(0);
}


char *get_lockfile_data(char *filename) {
    // Проверяем, что получили на вход имя файла
    if (filename == NULL) {
        log_error("There's no filename for .lck file");
        exit(1);
    }

    char *lockfile_name = (char *)malloc(strlen(filename) + sizeof(LOCK_SUFFIX) + 1); // +1 для завершающего нулевого символа
    if (lockfile_name == NULL) {
        log_error("Memory allocation failed");
        exit(1);
    }

    sprintf(lockfile_name, "%s%s", filename, LOCK_SUFFIX);
    return lockfile_name;
}


int lock(char* lockfile, char pid_buffer[5], int pid_buffer_length) {
    int fd = -1;

    // Проверям, существует ли файл .lck
    // Если не существует, то создаем
    // Если сущетвует, то ждем, пока удалится и пробуем еще раз
    while ((fd = open(lockfile, O_CREAT | O_EXCL | O_RDWR, 0666)) == -1) {
        if (errno != EEXIST) {
            log_error("Failed to create lock file");
            return -1;
        }
    }

    // Записываем свой PID в файл
    if (write(fd, pid_buffer, pid_buffer_length) != pid_buffer_length) {
        log_error("Failed to write PID to lock file");
        close(fd);
        return -1;
    }

    sleep(1);
    close(fd);
    return 0;
}


int unlock(char* lockfile, int pid_buffer_length) {
    char read_buffer[pid_buffer_length];
    int fd = open(lockfile, O_RDONLY, 0666);
    if (fd == -1) {
        log_error("Failed to open lock file for reading");
        return -1;
    }

    lseek(fd, 0, SEEK_SET);
    read(fd, read_buffer, pid_buffer_length);
    int readed_pid = atoi(read_buffer);
    close(fd);

    // Проверяем, что записанный в файле PID соответствует PID текущего процесса
    if (readed_pid != PID) {
        log_error("PID mismatch");
        return -1;
    }

    // Удаляем файл
    if (remove(lockfile) != 0) {
        log_error("Failed to remove lock file");
        return -1;
    }

    return 0;
}


int main(int argc, char *argv[]) {
    signal(SIGINT, killer);
     
    char* lockfile = get_lockfile_data(argv[1]); 
    PID = getpid();

    while (true) {
        char pid_buffer[10];
        sprintf(pid_buffer, "%d", PID);

        if (lock(lockfile, pid_buffer, strlen(pid_buffer)) != 0) {
            printf("Error: Error while locking");
            log_error("Error while locking");
            return 1;
        }

        if (unlock(lockfile, strlen(pid_buffer)) != 0) {
            printf("Error: Error while unlocking");
            log_error("Error while unlocking");
            return 1;
        }

        SUCCESSFUL_LOCKS_COUNT++;
    }

    free(lockfile);

    return 0;
}
