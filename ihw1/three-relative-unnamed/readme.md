# Отчёт для программы: Взаимодействие между тремя родственными процессами через неименованные каналы

## В отчёте присутствуют:
- [Код программы]()
- [Тестовые данные программые]()
- [Описание входных данных]()
- [Описание работы программы]()
- [Скриншоты демонстрирующие работу приложения через Терминал]()

## Код программы

Файл программы - [three-relative-unnamed.cpp](three-relative-unnamed.cpp)

## Тестовые данные программы

Тестовые данные находятся в папке [Data](three-relative-unnamed/Data/)

## Описание входных данных

В аргументах командной строки пользователю необходимо указать следующие данные:
- `-n` - Число N, необходимое для решения данной в варианте задачи. Если пользователь не укажет этот аргумент, или укажет n <= 0, то будет использовано значение по умолчанию
- `-in` - Путь к файлу с изначальными данными
- `-out` - Путь к файлу, куда надо записать результат работы

## Описание работы программы

 В начале реализована функция `findSequence`, выполняющая необходимые для выполнения задачи действия.
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

В начале функции `main` считываются данные из терминала и происходит проверка числа `n`, которая была затронута выше.
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

Далее создаются два канала `fd1` и `fd2` при помощи `pipe`
```cpp
int fd1[2];
int fd2[2];

if (pipe(fd1) == -1) {
    perror("pipe fd1");
    exit(EXIT_FAILURE);
}
if (pipe(fd2) == -1) {
    perror("pipe fd2");
    exit(EXIT_FAILURE);
}
```

Дальше идет большое ветвление `if`, которое отвечает за взаимодействие процессов между собой:
```cpp
pid_t pid1, pid2;
pid1 = fork();
if (pid1 < 0) {
    perror("fork 1");
    exit(EXIT_FAILURE);
} else if (pid1 == 0) {
  // work
} else {
  // work
}
```

Процесс 1 открывает указанный в командной строке файл для чтения. Далее, считывает содержимое файла в буфер
```cpp
FILE *fp = fopen(input_file, "r");
if (fp == NULL) {
    perror("open input file");
    exit(EXIT_FAILURE);
}
fgets(buffer, sizeof(buffer), fp);
fclose(fp);
```

Далее Процесс 1 отправляет данные через канал 1
```cpp
write(fd_write, buffer, strlen(buffer) + 1);
printf("Process 1 sent data: %s\n", buffer);
close(fd_write);
```

Когда работа переходит Процессу 2, он считывает данные из канала `fd1`
```cpp
read(fd_read, buffer, sizeof(buffer));
printf("Process 2 received data: %s\n", buffer);
close(fd_read);
```

Далее Процесс 2 вызывает `findSequence` для полученных данных и отправляет их через канал `fd2`
```cpp
findSequence(buffer, n, buffer);
if (buffer[0] == '\0') {
    strcpy(buffer, "No such sequence found.");
}

write(fd_write, buffer, strlen(buffer) + 1);
printf("Process 2 sent data: %s\n", buffer);
close(fd_write);
```

Процесс 3 получает данные из `fd2` и записывает их в указанный в аргумент командной строки выходной файл
```cpp
read(fd_read, buffer, sizeof(buffer));
printf("Process 3 received data: %s\n", buffer);
close(fd_read);

FILE *fp = fopen(output_file, "w");
if (fp == NULL) {
    perror("open output file");
    exit(EXIT_FAILURE);
}
fprintf(fp, "%s", buffer);
fclose(fp);
```

## Скриншоты демонстрирующие работу приложения через Терминал

### Пример 1. Некорректный ввод
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/e30f6288-e154-4bcb-9b63-bac9f6f43f19)

### Пример 2 

#### Ввод в терминале 
<img width="1085" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/472fa8ae-64e2-4d15-ac21-03bb30c47a11">

#### Содержимое файла input1.txt 
<img width="649" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/a09ae7ba-a4d6-411e-9391-2c4311141828">

#### Результат в терминале
<img width="1084" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/caa77480-78c4-45d2-a8ce-685433f11908">

#### Содержимое output1.txt
<img width="650" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/27d8314e-c5d3-49fd-a0df-07150226dac5">

### Пример 3. N > длины строки в файле

#### Ввод в терминале 
<img width="1083" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/5494d4a9-5621-4fda-9a6e-2094850dfaf0">

#### Содержимое файла input2.txt 
<img width="648" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/40719d22-182c-440a-bcb9-2647a0578e4b">

#### Результат в терминале
<img width="1081" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/fdde8837-24c4-4f81-b4c4-bb1fc158b632">

#### Содержимое output2.txt
<img width="647" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/8f2be92f-3ed4-40f9-826f-9abe902090a2">

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
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/ef7631e8-bcf5-4520-8ed1-5c94404b2ae2)

#### Содержимое файла input4.txt 
<img width="649" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/c9e82c7c-2610-4814-8cde-df2514aac9ff">

#### Результат в терминале
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/d0094d5a-3d44-40e2-bab9-747b9817e3eb)

#### Содержимое output4.txt
<img width="649" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/e9d334d2-ae51-43cb-ab3b-97108cae8651">

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
