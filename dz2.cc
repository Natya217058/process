#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEFAULT_MESSAGE "hello world"

int main(int argc, char *argv[]) {
    pid_t pid = getpid();
    printf("PID процесса: %d\n", pid);
    sleep(1);
    const char *message = (argc > 1) ? argv[1] : DEFAULT_MESSAGE;
    if (argc == 1) {
        execl(argv[0], argv[0], message, NULL);
        perror("Ошибка при выполнении exec");
        exit(1);
    }
    printf("Сообщение: %s\n", message);
   return 0;
}
