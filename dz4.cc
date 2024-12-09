#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

sem_t player1_sem;
sem_t player2_sem;
const char *moves[] = {
    "e2-e4", "e7-e5", "Ng1-f3", "Nb8-c6",
    "Bf1-c4", "Bf8-c5", "c2-c3", "Ng8-f6",
    "d2-d4", "e5-d4", "O-O", "O-O",
    NULL
};
int move_index = 0;
void *player1(void *arg) {
    while (1) {
        sem_wait(&player1_sem);
        if (moves[move_index] == NULL) {
            break;
        }
        printf("Player 1: %s\n", moves[move_index++]);
        sleep(1); 
        sem_post(&player2_sem);
    }
    sem_post(&player2_sem);
    pthread_exit(NULL);
}
void *player2(void *arg) {
    while (1) {
        sem_wait(&player2_sem);
        if (moves[move_index] == NULL) {
            break;
        }
        printf("Player 2: %s\n", moves[move_index++]);
        sleep(1); 
        sem_post(&player1_sem);
    }
    sem_post(&player1_sem);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;
    sem_init(&player1_sem, 0, 1); 
    sem_init(&player2_sem, 0, 0);
    pthread_create(&thread1, NULL, player1, NULL);
    pthread_create(&thread2, NULL, player2, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    sem_destroy(&player1_sem);
    sem_destroy(&player2_sem);
    printf("Game over!\n");
    return 0;
}
