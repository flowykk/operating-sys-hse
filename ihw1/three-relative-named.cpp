#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#define FIFO_FILE_1 "fifo1"
#define FIFO_FILE_2 "fifo2"
#define DATA_FILE "input.txt"
#define OUTPUT_FILE "output.txt"

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
            n = std::stoi(argv[++i]);
            printf("Inputed n is %d\n", n);
        }
    }
    if (n <= 0) {
        printf("Number n must be > 0!");
        printf("Program will use n = 1");
    }

    // Создание FIFOs
    if (mkfifo(FIFO_FILE_1, 0666) == -1) {
        printf("mkfifo FIFO_FILE_1");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(FIFO_FILE_2, 0666) == -1) {
        printf("mkfifo FIFO_FILE_2");
        exit(EXIT_FAILURE);
    }

    // Создание процессов
    pid_t pid1, pid2;
    pid1 = fork();
    if (pid1 < 0) {
        printf("fork 1");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) { // Процесс 1
        int fd1, fd2;
        char buffer[256];

        // Открытие FIFO для записи
        fd1 = open(FIFO_FILE_1, O_WRONLY);
        if (fd1 == -1) {
            printf("open FIFO 1 for writing");
            exit(EXIT_FAILURE);
        }

        // Чтение данных из файла
        FILE *fp = fopen(DATA_FILE, "r");
        if (fp == NULL) {
            printf("open input file");
            exit(EXIT_FAILURE);
        }
        fgets(buffer, sizeof(buffer), fp);
        fclose(fp);

        // Отправка данных через FIFO 1
        write(fd1, buffer, strlen(buffer) + 1);
        printf("Process 1 sent data: %s\n", buffer);
        close(fd1);

        exit(EXIT_SUCCESS);
    } else { // Процесс 2
        pid2 = fork();
        if (pid2 < 0) {
            printf("fork 2");
            exit(EXIT_FAILURE);
        } else if (pid2 == 0) {
            int fd1, fd2;
            char buffer[256];

            // Открытие FIFO для чтения
            fd1 = open(FIFO_FILE_1, O_RDONLY);
            if (fd1 == -1) {
                printf("open FIFO 1 for reading");
                exit(EXIT_FAILURE);
            }

            // Чтение данных из FIFO 1
            read(fd1, buffer, sizeof(buffer));
            printf("Process 2 received data: %s\n", buffer);
            close(fd1);

            // Добавление буквы 'A'
            findSequence(buffer, n, buffer);
            if (buffer[0] == '\0') {
                strcpy(buffer, "No such sequence found.");
            }


            // Отправка данных через FIFO 2
            fd2 = open(FIFO_FILE_2, O_WRONLY);
            if (fd2 == -1) {
                printf("open FIFO 2 for writing");
                exit(EXIT_FAILURE);
            }
            write(fd2, buffer, strlen(buffer) + 1);
            printf("Process 2 sent data: %s\n", buffer);
            close(fd2);

            exit(EXIT_SUCCESS);
        } else { // Процесс 3
            int fd;
            char buffer[256];

            // Открытие FIFO для чтения
            fd = open(FIFO_FILE_2, O_RDONLY);
            if (fd == -1) {
                printf("open FIFO 2 for reading");
                exit(EXIT_FAILURE);
            }

            // Чтение данных из FIFO 2
            read(fd, buffer, sizeof(buffer));
            printf("Process 3 received data: %s\n", buffer);
            close(fd);

            // Запись данных в файл
            FILE *fp = fopen(OUTPUT_FILE, "w");
            if (fp == NULL) {
                printf("open output file");
                exit(EXIT_FAILURE);
            }
            fprintf(fp, "%s", buffer);
            fclose(fp);

            // Удаление FIFOs
            if (unlink(FIFO_FILE_1) == -1) {
                perror("unlink FIFO_FILE_1");
                exit(EXIT_FAILURE);
            }
            if (unlink(FIFO_FILE_2) == -1) {
                perror("unlink FIFO_FILE_2");
                exit(EXIT_FAILURE);
            }

            printf("Data processed successfully.\n");

            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
