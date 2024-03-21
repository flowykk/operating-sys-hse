// server.cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <cstdlib>
#include <signal.h>

int shm_id;
int *share;

void signalHandler(int signal) {
    if (signal == SIGALRM) {
        printf("Timer expired, exiting...\n");

        if (shmdt(share) == -1) {
            perror("shmdt()");
            exit(3);
        }

        if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
            perror("shmctl()");
            exit(4);
        }

        exit(0);
    }
}

int main() {
    shm_id = shmget (0x2FF, sizeof(int), 0666 | IPC_CREAT);
    if(shm_id == -1){
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

    while(1){
        printf("%d\n", *share);
        sleep(1);
    }

    return 0;
}
