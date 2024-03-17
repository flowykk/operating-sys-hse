# Отчёт для программы: Взаимодействие между двумя родственными процессами через именованные каналы

## Код программы

Файл программы - [two-relative-named.cpp](two-relative-named.cpp)

## Тестовые данные программы

Тестовые данные находятся в папке `../ihw1/Data/`

## Описание входных данных

В аргументах командной строки пользователю необходимо указать следующие данные:
- `-n` - Число N, необходимое для решения данной в варианте задачи. Если пользователь не укажет этот аргумент, или укажет n <= 0, то будет использовано значение по умолчанию
- `-in` - Путь к файлу с изначальными данными
- `-out` - Путь к файлу, куда надо записать результат работы

## Описание работы программы

Для начала стоит учесть, что используются два именованных канала, которые передают данные между тремя родственными процессами. Названия каналов - `fifo1` и `fifo2` соответственно.

Теперь посмотрим код. В начале реализована функция `findSequence`, выполняющая необходимые для выполнения задачи действия.
```cpp
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
```

Далее в функции `main` считываются данные из терминала и происходит проверка числа `n`, которая была затронута выше.
```cpp
for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-n") == 0) {
        n = std::stoi(argv[++i]);
        printf("Inputed n is %d\n", n);
    } else if (strcmp(argv[i], "-in") == 0) {
        input_file = argv[++i];
    } else if (strcmp(argv[i], "-out") == 0) {
        output_file = argv[++i];
    }
}
if (n <= 0) {
    printf("Number n must be > 0!");
    printf("Program will use n = 1");
    n = 1;
}
```

Далее происходит создание двух каналов и обработка ошибок, уже описанное ранее.
```cpp
if (mkfifo(FIFO_FILE_1, 0666) == -1) {
    printf("mkfifo FIFO_FILE_1");
    exit(EXIT_FAILURE);
}
if (mkfifo(FIFO_FILE_2, 0666) == -1) {
    printf("mkfifo FIFO_FILE_2");
    exit(EXIT_FAILURE);
}
```

После этого идёт большое условие `if`, и создаются три процесса, между которыми будут передаваться данные:
```cpp
pid_t pid1, pid2;
pid1 = fork();
if (pid1 < 0) {
    // fail
} else if (pid1 == 0) { // Процесс 1
    // work
} else { // Процесс 2
    // work
}
```

Далее открывается именованный канал `FIFO_FILE_1` для записи. А затем он открывает указанный входной файл `input_file` и считывает его содержимое в буфер.
```cpp
fd1 = open(FIFO_FILE_1, O_WRONLY);
if (fd1 == -1) {
    perror("open FIFO 1 for writing");
    exit(-1);
}

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
```

После считывания данных из файла, они отправляются через именованный канал `FIFO_FILE_1`. Затем процесс 1 открывает именованный канал `FIFO_FILE_2` для чтения и считывает данные из него.
```cpp
write(fd1, buffer, strlen(buffer) + 1);
printf("Process 1 sent data: %s\n", buffer);
close(fd1);

fd2 = open(FIFO_FILE_2, O_RDONLY);
if (fd2 == -1) {
    perror("open FIFO 2 for reading");
    exit(-1);
}

read(fd2, buffer, sizeof(buffer));
printf("Process 1 received data: %s\n", buffer);
close(fd2);
```

В конце своей работы, Процесс 1 записывает полученные после работы Процесса 2 данные в указанный выходной файл `output_file`. Затем он удаляет именованные каналы `FIFO_FILE_1 и FIFO_FILE_2`, используя функцию unlink.
```cpp
FILE *out_fp = fopen(output_file, "w");
if (out_fp == NULL) {
    perror("open output file");
    exit(-1);
}
fprintf(out_fp, "%s", buffer);
fclose(out_fp);

if (unlink(FIFO_FILE_1) == -1) {
    perror("unlink FIFO_FILE_1");
    exit(-1);
}
if (unlink(FIFO_FILE_2) == -1) {
    perror("unlink FIFO_FILE_2");
    exit(-1);
}
```

Теперь рассмотрим работу Процесса 2. Блок кода ниже открывает именованный канал `FIFO_FILE_1` для чтения, читает данные из него и сохраняет их в буфер.
```cpp
fd1 = open(FIFO_FILE_1, O_RDONLY);
if (fd1 == -1) {
    perror("open FIFO 1 for reading");
    exit(-1);
}

read(fd1, buffer, sizeof(buffer));
printf("Process 2 received data: %s\n", buffer);
close(fd1);
```

После чтения данных Процесс 2 обрабатывает эти данные, вызывает findSequence для буфера, и открывает именованный канал `FIFO_FILE_2` для записи и отправляет обработанные данные. А работа переходит к Процессу 1
```cpp
findSequence(buffer, n, buffer);
if (buffer[0] == '\0') {
    strcpy(buffer, "No such sequence found.");
}

fd2 = open(FIFO_FILE_2, O_WRONLY);
if (fd2 == -1) {
    perror("open FIFO 2 for writing");
    exit(-1);
}

write(fd2, buffer, strlen(buffer) + 1);
printf("Process 2 sent data: %s\n", buffer);
close(fd2);
```


## Скриншоты демонстрирующие работу приложения через Терминал

### Пример 1. Некорректный ввод
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/5dedfeda-e2e1-4ddf-8d4e-5845162fd536)

### Пример 2 

#### Содержимое файла input1.txt 
<img width="649" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/a09ae7ba-a4d6-411e-9391-2c4311141828">

#### Результат в терминале
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/8f6abf5d-e679-4c48-972c-8a2058e870dd)

#### Содержимое output1.txt
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/97451972-aea9-4a47-a7b9-881409ff87fd)

### Пример 3. N > длинны строки в файле

#### Содержимое файла input2.txt 
<img width="648" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/40719d22-182c-440a-bcb9-2647a0578e4b">

#### Результат в терминале
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/4c77b8de-3a9a-41b6-915a-2aeee65f4ffc)

#### Содержимое output2.txt
<img width="647" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/be0691a9-d336-48c5-82dd-fb3e0df09836">

### Пример 4.

#### Содержимое файла input3.txt 
<img width="647" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/9c2f6005-150d-4077-b2a4-031ba4e542d8">

#### Результат в терминале
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/1a447655-49eb-4f01-8444-3a16c4b62ef7)

#### Содержимое output3.txt
<img width="647" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/c5793740-638b-4e6d-8a7c-60719cfe96d8">

### Пример 5. 1000 символов

#### Содержимое файла input4.txt 
<img width="649" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/c9e82c7c-2610-4814-8cde-df2514aac9ff">

#### Результат в терминале
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/e79fa7d6-7b3a-4b1c-98de-557e68c1d7eb)

#### Содержимое output4.txt
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/3f15c4c6-109f-4e3b-8476-bef528765115)

### Пример 6. 5000 символов

#### Содержимое файла input5.txt 
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/361d5a74-f87b-4e2b-bae3-b94d4ad12d67)

#### Результат в терминале
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/9fcf7a25-854e-461e-86c7-56af9a23f2ec)

![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/6f41a22b-629f-4e2b-b4c7-6a8793e34d59)

#### Содержимое output5.txt
<img width="647" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/a2ee5f9f-b492-47c5-b2d0-3f577c15cd52">
