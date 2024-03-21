// client.cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

int shm_id;
int *share;

void signalHandler(int signal) {
    if (signal == SIGALRM) {
        printf("Timer expired, exiting...\n");

        // Отключение сегмента разделяемой памяти
        if (shmdt(share) == -1) {
            perror("shmdt()");
            exit(3);
        }

        exit(0);
    }
}

int main(){
    srand(time(NULL));

    shm_id = shmget (0x2FF, sizeof(int), 0666 | IPC_CREAT);
    if(shm_id < 0){
        perror("shmget()");
        exit(1);
    }

    share = (int *)shmat(shm_id, 0, 0);
    if(share == NULL){
        perror("shmat()");
        exit(2);
    }

    signal(SIGALRM, signalHandler);

    alarm(5);

    int num;
    while(1){
        num = random() % 1000;
        *share = num;
        printf("write a random number %d\n", num);
        sleep(1);
    }

    return 0;
}
