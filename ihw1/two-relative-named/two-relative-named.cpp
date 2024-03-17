#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIFO_FILE_1 "fifo1"
#define FIFO_FILE_2 "fifo2"

int n = 1;
const char *input_file;
const char *output_file;

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

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            n = atoi(argv[++i]);
            printf("Inputed n is %d\n", n);
        } else if (strcmp(argv[i], "-in") == 0) {
            input_file = argv[++i];
        } else if (strcmp(argv[i], "-out") == 0) {
            output_file = argv[++i];
        }
    }
    if (n <= 0) {
        printf("Number n must be > 0! Program will use n = 1\n");
        n = 1;
    }

    // Создание FIFOs
    if (mkfifo(FIFO_FILE_1, 0666) == -1) {
        perror("mkfifo FIFO_FILE_1");
        exit(-1);
    }
    if (mkfifo(FIFO_FILE_2, 0666) == -1) {
        perror("mkfifo FIFO_FILE_2");
        exit(-1);
    }

    // Создание процессов
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(-1);
    } else if (pid == 0) { // Процесс 1
        int fd1, fd2;
        char buffer[5000];

        // Открытие FIFO 1 для записи
        fd1 = open(FIFO_FILE_1, O_WRONLY);
        if (fd1 == -1) {
            perror("open FIFO 1 for writing");
            exit(-1);
        }

        // Чтение данных из файла
        FILE *fp = fopen(input_file, "r");
        if (fp == NULL) {
            perror("open input file");
            exit(-1);
        }
        fgets(buffer, sizeof(buffer), fp);

        if (strlen(buffer) == 0) {
            perror("Your input-file`s data is too small");
            exit(-1);
        }

        fclose(fp);

        // Отправка данных через FIFO 1
        write(fd1, buffer, strlen(buffer) + 1);
        printf("Process 1 sent data: %s\n", buffer);
        close(fd1);

        // Открытие FIFO 2 для чтения
        fd2 = open(FIFO_FILE_2, O_RDONLY);
        if (fd2 == -1) {
            perror("open FIFO 2 for reading");
            exit(-1);
        }

        // Чтение данных из FIFO 2
        read(fd2, buffer, sizeof(buffer));
        printf("Process 1 received data: %s\n", buffer);
        close(fd2);

        // Запись данных в файл
        FILE *out_fp = fopen(output_file, "w");
        if (out_fp == NULL) {
            perror("open output file");
            exit(-1);
        }
        fprintf(out_fp, "%s", buffer);
        fclose(out_fp);

        // Удаление FIFOs
        if (unlink(FIFO_FILE_1) == -1) {
            perror("unlink FIFO_FILE_1");
            exit(-1);
        }
        if (unlink(FIFO_FILE_2) == -1) {
            perror("unlink FIFO_FILE_2");
            exit(-1);
        }

        exit(EXIT_SUCCESS);
    } else { // Процесс 2
        int fd1, fd2;
        char buffer[5000];

        // Открытие FIFO 1 для чтения
        fd1 = open(FIFO_FILE_1, O_RDONLY);
        if (fd1 == -1) {
            perror("open FIFO 1 for reading");
            exit(-1);
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

        // Открытие FIFO 2 для записи
        fd2 = open(FIFO_FILE_2, O_WRONLY);
        if (fd2 == -1) {
            perror("open FIFO 2 for writing");
            exit(-1);
        }

        // Отправка данных через FIFO 2
        write(fd2, buffer, strlen(buffer) + 1);
        printf("Process 2 sent data: %s\n", buffer);
        close(fd2);

        exit(EXIT_SUCCESS);
    }

    return 0;
}
