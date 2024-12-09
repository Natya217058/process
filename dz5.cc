#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

void sigint_handler(int signo) {
    printf("Thread 2: Received SIGINT (Ctrl+C)\n");
}
void *thread1_func(void *arg) {
    printf("Thread 1: Blocking all signals\n");
    sigset_t set;
    sigfillset(&set);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    while (1) {
        sleep(1);
    }
}

void *thread2_func(void *arg) {
    printf("Thread 2: Waiting for SIGINT (Ctrl+C)\n");
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    while (1) {
        pause(); 
    }
} 
    void *thread3_func(void *arg) {
    printf("Thread 3: Waiting for SIGQUIT (Ctrl+\\)\n");
    sigset_t set, oldset;
    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    int signo;
    while (1) {
        sigwait(&set, &signo); 
        if (signo == SIGQUIT) {
            printf("Thread 3: Received SIGQUIT (Ctrl+\\)\n");
            break;
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, thread1_func, NULL);
    pthread_create(&thread2, NULL, thread2_func, NULL);
    pthread_create(&thread3, NULL, thread3_func, NULL);
    pthread_join(thread3, NULL);

    printf("Main thread: Exiting program\n");
    return 0;
}
