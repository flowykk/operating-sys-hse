#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define INITIAL_FILENAME "a"
#define MAX_FILENAME_LENGTH 1024

int main() {
    int depth = 0;
    char filename[MAX_FILENAME_LENGTH];
    char linkname[MAX_FILENAME_LENGTH];
    int fd;

    fd = open(INITIAL_FILENAME, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("error: Can't create file");
        return 1;
    }
    close(fd);

    snprintf(filename, MAX_FILENAME_LENGTH, INITIAL_FILENAME);
    while (true) {
        snprintf(linkname, MAX_FILENAME_LENGTH, "link_%d", depth);
        if (symlink(filename, linkname) == -1) {
            perror("error: Can't create link");
            break;
        }

        fd = open(linkname, O_RDONLY);
        if (fd == -1) {
            perror("error: Can't open file");
            break;
        }
        close(fd);

        snprintf(filename, MAX_FILENAME_LENGTH, "%s", linkname);
        depth++;
    }

    printf("Recursion depth reached: %d\n", depth);
    for (int i = 0; i <= depth; i++) {
        snprintf(linkname, MAX_FILENAME_LENGTH, "l_%d", i);
        unlink(linkname);
    }

    unlink(INITIAL_FILENAME);
    return 0;
}
