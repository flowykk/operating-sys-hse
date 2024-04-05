#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "message.h"

void sys_err(const char *message) {
    perror(message);
    exit(1);
}

const char* SHM_NAME = "myshm";

int main() {
    int shm_id;
    message_t *msg_p;

    if ((shm_id = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666)) == -1) {
        perror("shmget error");
        exit(1);
    }

    if (ftruncate(shm_id, sizeof(message_t)) == -1) {
        perror("ftruncate error");
        return 1;
    } else {
        printf("Memory size set and = %lu\n", sizeof(message_t));
    }

    msg_p = (message_t *)mmap(NULL, sizeof(message_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
    msg_p->status = CONTINUE;

    while (1) {
        sleep(1);
        printf("%d\n", msg_p->num);
        if (msg_p->status == STOP) {
            printf("%s\n", "Server completed work");
            break;
        }
    }

    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        return 1;
    }

    return 0;
}
