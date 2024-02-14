#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>

const int buffSizeSmall = 16;

int main(int argc, char* argv[]) {

    int size = buffSizeSmall;
    char buffer[size];
    ssize_t read_bytes;

    int file_to_read; // file to read text from
    int file_to_write; // file to write text to

    if ((file_to_read = open(argv[1], O_RDONLY)) < 0) {
        printf("Cannot open the read-file\n");
        exit(-1);
    }

    if ((file_to_write = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
        printf("Cannot open the write-file\n");
        exit(-1);
    }

    do {
        read_bytes = read(file_to_read, buffer, size);
        if (read_bytes == -1) {
            printf("Cannot read the file!\n");
            exit(-1);
        }
        write(file_to_write, buffer, read_bytes);
    } while (read_bytes == size);

    if (close(file_to_read) < 0) {
        printf("Can't close file");
        exit(-1);
    }

    if (close(file_to_write) < 0) {
        printf("Can't close file");
        exit(-1);
    }

    printf("File was copied successfully!");
    return 0;
}
