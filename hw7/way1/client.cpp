#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include "message.h"

const char* SHM_NAME = "myshm";

int main() {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    int shm_id;
    message_t *msg_p;
    int num;

    if ((shm_id = shm_open(SHM_NAME, O_RDWR, 0666)) == -1) {
        perror("shmget()");
        exit(1);
    }

    msg_p = (message_t *) mmap(NULL, sizeof(message_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);

    if (msg_p == (message_t *)-1 ) {
        printf("Error getting pointer to addrd memory\n");
        return 1;
    }

    while (1) {
        num = random() % 1000;
        msg_p->num = num;
        printf("write a random number %d\n", msg_p->num);

        if (num % 7 == 0) {
            msg_p->status = STOP;
            break;
        }
        sleep(1);
    }

    puts("The client was completed his work\n");
    return 0;
}
