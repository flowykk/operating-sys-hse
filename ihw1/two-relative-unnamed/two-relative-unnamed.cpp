#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

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

    int fd1[2]; // Канал для передачи данных от процесса 1 к процессу 2
    int fd2[2]; // Канал для передачи данных от процесса 2 к процессу 1

    // Создание каналов
    if (pipe(fd1) == -1) {
        perror("pipe fd1");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd2) == -1) {
        perror("pipe fd2");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Процесс 1
        close(fd1[0]); // Закрыть чтение из канала fd1
        close(fd2[1]); // Закрыть запись в канал fd2

        char buffer[5000];

        // Чтение данных из файла
        FILE *fp = fopen(input_file, "r");
        if (fp == NULL) {
            perror("open input file");
            exit(EXIT_FAILURE);
        }
        fgets(buffer, sizeof(buffer), fp);
        fclose(fp);

        // Отправка данных через канал fd1
        write(fd1[1], buffer, strlen(buffer) + 1);
        printf("Process 1 sent data: %s\n", buffer);

        // Чтение обработанных данных из канала fd2
        int num_bytes_read = read(fd2[0], buffer, sizeof(buffer));
        if (num_bytes_read < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        printf("Process 1 received data: %s\n", buffer);

        // Запись данных в файл
        FILE *out_fp = fopen(output_file, "w");
        if (out_fp == NULL) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        fprintf(out_fp, "%s", buffer);
        fclose(out_fp);

        exit(EXIT_SUCCESS);
    } else { // Процесс 2
        close(fd1[1]); // Закрыть запись в канал fd1
        close(fd2[0]); // Закрыть чтение из канала fd2

        char buffer[5000];

        // Чтение данных из канала fd1
        int num_bytes_read = read(fd1[0], buffer, sizeof(buffer));
        if (num_bytes_read < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        printf("Process 2 received data: %s\n", buffer);

        // Обработка данных
        findSequence(buffer, n, buffer);
        if (buffer[0] == '\0') {
            strcpy(buffer, "No such sequence found.");
        }

        // Отправка обработанных данных через канал fd2
        write(fd2[1], buffer, strlen(buffer) + 1);
        printf("Process 2 sent data: %s\n", buffer);

        exit(EXIT_SUCCESS);
    }

    return 0;
}
