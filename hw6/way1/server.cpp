// server.cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <cstdlib>

int main() {
    int shm_id;
    int *share;

    shm_id = shmget (0x2FF, 2 * sizeof(int), 0666 | IPC_CREAT);
    if(shm_id == -1){
        perror("shmget()");
        exit(1);
    }

    share = (int *)shmat(shm_id, 0, 0);
    if(share == NULL){
        perror("shmat()");
        exit(2);
    }

    while(1){
        if (share[1] == 1) {
            printf("received STOP-number  %d\n", share[0]);
            break;
        } else {
            printf("%d\n", share[0]);
            sleep(1);
        }
    }

    if (shmdt(share) == -1) {
        perror("shmdt()");
        exit(3);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl()");
        exit(4);
    }

    return 0;
}
