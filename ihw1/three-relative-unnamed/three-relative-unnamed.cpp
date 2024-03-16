#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

int n = 1;

void findSequence(const char* input, int N, char* sequence) {
    size_t length = strlen(input);
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
                sequence[k] = input[i + k];
            }
            sequence[N] = '\0';
            return;
        }
    }

    sequence[0] = '\0';
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            n = atoi(argv[++i]);
            printf("Inputed n is %d\n", n);
        }
    }
    if (n <= 0) {
        printf("Number n must be > 0!");
        printf("Program will use n = 1");
        n = 1;
    }

    int fd1[2]; // Канал 1 для процессов 1 и 2
    int fd2[2]; // Канал 2 для процессов 2 и 3

    // Создание каналов
    if (pipe(fd1) == -1) {
        perror("pipe fd1");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd2) == -1) {
        perror("pipe fd2");
        exit(EXIT_FAILURE);
    }

    // Создание процессов
    pid_t pid1, pid2;
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork 1");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) { // Процесс 1
        close(fd1[0]); // Закрыть чтение в процессе 1
        close(fd2[0]); // Закрыть чтение в процессе 1
        close(fd2[1]); // Закрыть запись в процессе 1

        int fd_write = fd1[1]; // Открыть запись в процессе 1
        char buffer[BUFFER_SIZE];

        // Чтение данных из файла
        FILE *fp = fopen("Data/input.txt", "r");
        if (fp == NULL) {
            perror("open input file");
            exit(EXIT_FAILURE);
        }
        fgets(buffer, sizeof(buffer), fp);
        fclose(fp);

        // Отправка данных через канал 1
        write(fd_write, buffer, strlen(buffer) + 1);
        printf("Process 1 sent data: %s\n", buffer);
        close(fd_write);

        exit(EXIT_SUCCESS);
    } else { // Процесс 2
        pid2 = fork();
        if (pid2 < 0) {
            perror("fork 2");
            exit(EXIT_FAILURE);
        } else if (pid2 == 0) {
            close(fd1[1]); // Закрыть запись в процессе 2
            close(fd2[0]); // Закрыть чтение в процессе 2

            int fd_read = fd1[0]; // Открыть чтение в процессе 2
            int fd_write = fd2[1]; // Открыть запись в процессе 2
            char buffer[BUFFER_SIZE];

            // Чтение данных из канала 1
            read(fd_read, buffer, sizeof(buffer));
            printf("Process 2 received data: %s\n", buffer);
            close(fd_read);

            // Добавление буквы 'A'
            findSequence(buffer, n, buffer);
            if (buffer[0] == '\0') {
                strcpy(buffer, "No such sequence found.");
            }

            // Отправка данных через канал 2
            write(fd_write, buffer, strlen(buffer) + 1);
            printf("Process 2 sent data: %s\n", buffer);
            close(fd_write);

            exit(EXIT_SUCCESS);
        } else { // Процесс 3
            close(fd1[0]); // Закрыть чтение в процессе 3
            close(fd1[1]); // Закрыть запись в процессе 3
            close(fd2[1]); // Закрыть запись в процессе 3

            int fd_read = fd2[0]; // Открыть чтение в процессе 3
            char buffer[BUFFER_SIZE];

            // Чтение данных из канала 2
            read(fd_read, buffer, sizeof(buffer));
            printf("Process 3 received data: %s\n", buffer);
            close(fd_read);

            // Запись данных в файл
            FILE *fp = fopen("Data/output.txt", "w");
            if (fp == NULL) {
                perror("open output file");
                exit(EXIT_FAILURE);
            }
            fprintf(fp, "%s", buffer);
            fclose(fp);

            printf("Data processed successfully.\n");

            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
