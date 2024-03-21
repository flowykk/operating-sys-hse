// client.cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int shm_id;
    int *share;
    int num;

    srand(time(NULL));
    shm_id = shmget (0x2FF, 2 * sizeof(int), 0666 | IPC_CREAT);
    if(shm_id < 0){
        perror("shmget()");
        exit(1);
    }

    share = (int *)shmat(shm_id, 0, 0);
    if(share == NULL){
        perror("shmat()");
        exit(2);
    }

    while(1){
        num = random() % 1000;
        share[0] = num;
        printf("write a random number %d\n", num);
        if (num % 5 == 0) {
            printf("received STOP-number  %d\n", num);
            share[1] = 1; /
            break;
        }
        sleep(1);
    }

    if (shmdt(share) == -1) {
        perror("shmdt()");
        exit(3);
    }

    return 0;
}
