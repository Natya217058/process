#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define FIFO_NAME "signal_fifo"

void sigabrt_handler(int signum) {
    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd < 0) {
        perror("Не удалось открыть FIFO");
        exit(1);
    }
    const char *message = "Получен сигнал SIGABRT\n";
    write(fd, message, strlen(message));
    close(fd);
    exit(0);
}
int main() {
    if (signal(SIGABRT, sigabrt_handler) == SIG_ERR) {
        perror("Ошибка установки обработчика сигнала");
        exit(1);
    }
    if (access(FIFO_NAME, F_OK) == -1) {
        if (mkfifo(FIFO_NAME, 0666) != 0) {
            perror("Не удалось создать FIFO");
            exit(1);
        }
    }
    printf("Ожидание сигнала SIGABRT. PID процесса: %d\n", getpid());
    while (1) {
        pause();
    }
    return 0;
}
