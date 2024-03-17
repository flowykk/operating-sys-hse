# Отчёт для программы: Взаимодействие между двумя родственными процессами через именованные каналы

## В отчёте присутствуют:
- [Код программы]()
- [Тестовые данные программые]()
- [Описание входных данных]()
- [Описание работы программы]()
- [Скриншоты демонстрирующие работу приложения через Терминал]()

## Код программы

Файл программы - [two-relative-named.cpp](two-relative-named.cpp)

## Тестовые данные программы

Тестовые данные находятся в папке [Data](Data/)

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
<img width="1085" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/8e8dca85-20e6-4748-80a4-a5c9f25ac9fe">

### Пример 2 

#### Ввод в терминале 
<img width="1085" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/472fa8ae-64e2-4d15-ac21-03bb30c47a11">

#### Содержимое файла input1.txt 
<img width="649" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/a09ae7ba-a4d6-411e-9391-2c4311141828">

#### Результат в терминале
<img width="1084" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/caa77480-78c4-45d2-a8ce-685433f11908">

#### Содержимое output1.txt
<img width="650" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/27d8314e-c5d3-49fd-a0df-07150226dac5">

### Пример 3. N > длинны строки в файле

#### Ввод в терминале 
<img width="1083" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/5494d4a9-5621-4fda-9a6e-2094850dfaf0">

#### Содержимое файла input2.txt 
<img width="648" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/40719d22-182c-440a-bcb9-2647a0578e4b">

#### Результат в терминале
<img width="1081" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/fdde8837-24c4-4f81-b4c4-bb1fc158b632">

#### Содержимое output2.txt
<img width="647" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/be0691a9-d336-48c5-82dd-fb3e0df09836">

### Пример 4. Пустой файл

#### Ввод в терминале 
<img width="1086" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/3ec4c5f2-5965-43c9-b611-d793451d7fe8">

#### Содержимое файла input3.txt 
<img width="647" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/9c2f6005-150d-4077-b2a4-031ba4e542d8">

#### Результат в терминале
<img width="1080" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/34a534bb-1a5e-4d5c-9710-cd3441acf22c">

#### Содержимое output3.txt
<img width="647" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/c5793740-638b-4e6d-8a7c-60719cfe96d8">

### Пример 5. 1000 символов

#### Ввод в терминале 
<img width="1083" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/a14c8fe8-7248-42bc-9bb9-7d3e9c333e8c">

#### Содержимое файла input4.txt 
<img width="649" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/c9e82c7c-2610-4814-8cde-df2514aac9ff">

#### Результат в терминале
<img width="1079" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/c43882ac-27c8-46b1-b8d2-e48afa2880d2">

#### Содержимое output4.txt
<img width="646" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/6c75a7f0-c77a-440e-9e9e-d7bf24d8a2bd">

### Пример 6. 5000 символов

#### Ввод в терминале 
<img width="1082" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/5af77554-3bb5-4465-b9fc-261e54651c40">

#### Содержимое файла input5.txt 
<img width="1083" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/e9ce4b24-fc1e-48fd-a19a-6e41d34c4fc5">

#### Результат в терминале
<img width="1083" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/11c19852-3a18-409d-8f18-a1dadc7c18a7">

<img width="1083" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/0703dc66-e14a-4a32-9299-34a0ee6b1412">

#### Содержимое output5.txt
<img width="647" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/a2ee5f9f-b492-47c5-b2d0-3f577c15cd52">
