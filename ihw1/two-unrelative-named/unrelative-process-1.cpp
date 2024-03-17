#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>

const char *fifo1 = "/tmp/fifo1";
const char *fifo2 = "/tmp/fifo2";

#define BUFFER_SIZE 5000

const char *input_file;
const char *output_file;

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-in") == 0) {
            input_file = argv[++i];
        } else if (strcmp(argv[i], "-out") == 0) {
            output_file = argv[++i];
        }
    }

    char buffer[BUFFER_SIZE];

    mknod(fifo1, S_IFIFO | 0666, 0);
    mknod(fifo2, S_IFIFO | 0666, 0);

    int fd = open(input_file, O_RDONLY);
    if (fd < 0) {
        printf("Can\'t open file for reading\n");
        exit(-1);
    }
    read(fd, buffer, BUFFER_SIZE);
    close(fd);

    fd = open(fifo1, O_WRONLY);
    write(fd, buffer, BUFFER_SIZE);
    close(fd);
    printf("Process 1 sent data: %s\n", buffer);

    fd = open(fifo2, O_RDONLY);
    read(fd, buffer, sizeof(buffer));
    close(fd);
    printf("Process 1 received data: %s\n", buffer);

    FILE *out_fp = fopen(output_file, "w");
    if (out_fp == NULL) {
        perror("open output file");
        exit(EXIT_FAILURE);
    }
    fprintf(out_fp, "%s", buffer);
    fclose(out_fp);

    unlink(fifo1);
    unlink(fifo2);
    return 0;
}
