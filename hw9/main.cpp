#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>

int fd[2];
int semid;

void handler(int sig) {
    close(fd[0]);
    close(fd[1]);
    if (semctl(semid, 0, IPC_RMID, 0) < 0) {
        printf("Can\'t delete semaphore\nIt may be already deleted\n");
        exit(-1);
    }
    printf("Semaphore deleted\n");
    exit(0);
}

void child(int* pd, int semid) {
    while(true) {
        char result[20] = "\0";
        read(pd[0], result, 15);
        printf("child: %s\n", result);


        struct sembuf child_buf =
                {.sem_num = 0, .sem_op = 1, .sem_flg = 0};
        if(semop(semid, &child_buf, 1) < 0){
            printf("Can\'t add 1 to semaphor\n");
            exit(-1);
        }
        write(pd[1], "child to parent", 15);
        sleep(1);
    }
}

void parent(int* pd, int semid) {
    while(true) {
        write(pd[1], "parent to child", 15);

        struct sembuf parent_buf =
                {.sem_num = 0, .sem_op = -1, .sem_flg = 0};
        if(semop(semid, &parent_buf, 1) < 0){
            printf("Can\'t sub 1 from semaphor\n");
            exit(-1);
        }
        char result[20] = "\0";
        read(pd[0], result, 15);
        printf("parent: %s\n", result);
        sleep(1);
    }
}

int main() {
    if(pipe(fd) < 0) {
        printf("Pipe error\n");
        return -1;
    }

    char pathname[]="pipe-sem.c";
    key_t key = ftok(pathname, 0);

    if((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL) ) < 0){
        printf("Can\'t create semaphore\n");
        return -1;
    }
    semctl(semid, 0, SETVAL, 0);

    signal(SIGINT, handler);

    pid_t pid = fork();
    if (pid < 0) {
        printf("Fork error\n");
        return -1;
    } else if (pid == 0) {
        child(fd, semid);
    } else {
        close(fd[0]);
        close(fd[1]);
        parent(fd, semid);
        wait(0);
    }
    return 0;
}
