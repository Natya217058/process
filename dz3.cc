#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

#define SHM_KEY 12345
#define MAX_TIME 10 

typedef struct {
    double white_time;
    double black_time;
    int last_player;
} SharedData;

void end_game(const char *message) {
    printf("%s\n", message);
    exit(0);
}

double get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (tv.tv_usec / 1000000.0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [white|black]\n", argv[0]);
        return 1;
    }

    int player = (argv[1][0] == 'w') ? 0 : 1; 
    int shmid = shmget(SHM_KEY, sizeof(SharedData), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }
    SharedData *data = (SharedData *)shmat(shmid, NULL, 0);
    if (data == (void *)-1) {
        perror("shmat");
        return 1;
    }
    if (player == 0 && data->white_time == 0 && data->black_time == 0) {
        data->white_time = 0;
        data->black_time = 0;
        data->last_player = -1;
    }
    if (data->last_player == player) {
        printf("Нельзя ходить дважды подряд!\n");
        shmdt(data);
        return 1;
    }
    double start_time = get_current_time();
    double elapsed_time = 0;

    if (data->last_player != -1) {
        elapsed_time = start_time - (player == 0 ? data->black_time : data->white_time);
    }
    if (player == 0) {
        data->white_time += elapsed_time;
        if (data->white_time > MAX_TIME) {
            end_game("Техническое поражение белых");
        }
    } else {
        data->black_time += elapsed_time;
        if (data->black_time > MAX_TIME) {
            end_game("Техническое поражение чёрных");
        }
    }
    if (player == 0) {
        data->white_time = start_time;
    } else {
        data->black_time = start_time;
    }
    data->last_player = player;

    printf("Ход сделан. Время белых: %.2f, Время чёрных: %.2f\n", data->white_time, data->black_time);

    shmdt(data);
    return 0;
}
