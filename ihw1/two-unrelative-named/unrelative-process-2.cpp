#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>

const char *fifo1 = "/tmp/fifo1";
const char *fifo2 = "/tmp/fifo2";

void findSequence(const char* input, int N, char* sequence) {
    size_t length = strlen(input);

    if (length == 0) {
        sequence[0] = '\0';
        return;
    }

    for (int i = 0; i <= length - N; ++i) {
        bool found = true;
        for (int j = i + 1; j < i + N; ++j) {
            if (input[j] >= input[j - 1]) {
                found = false;
                break;
            }
        }
        if (found) {
            for (int k = 0; k < N; ++k) {
                if (i + k >= length) {
                    return;
                }
                sequence[k] = input[i + k];
            }
            sequence[N] = '\0';
            return;
        }
    }

    sequence[0] = '\0';
}

#define BUFFER_SIZE 5000

int n = 1;

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            n = atoi(argv[++i]);
            printf("Inputed n is %d\n", n);
        }
    }
    if (n <= 0) {
        printf("Number n must be > 0! Program will use n = 1\n");
        n = 1;
    }

    char buffer[BUFFER_SIZE];

    int fd = open(fifo1, O_RDONLY);
    read(fd, buffer, BUFFER_SIZE);
    close(fd);

    findSequence(buffer, n, buffer);

    fd = open(fifo2, O_WRONLY);
    write(fd, buffer, sizeof(buffer));
    close(fd);

    printf("Process 2 sent data: %s\n", buffer);

    return 0;
}
