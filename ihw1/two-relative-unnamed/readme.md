# Отчёт для программы: Взаимодействие между двумя родственными процессами через неименованные каналы

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

Далее создаются два канала `fd1` и `fd2` при помощи `pipe` для передачи данных между двумя процессами
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

Процесс 1 открывает файл `input_file` для чтения, считывает данные из него, а затем закрывает файл
```cpp
FILE *fp = fopen(input_file, "r");
if (fp == NULL) {
    perror("open input file");
    exit(EXIT_FAILURE);
}
fgets(buffer, sizeof(buffer), fp);
fclose(fp);
```

Далее Процесс 1 записывает данные из буфера в канал `fd1` с помощью функции `write`. Позже эти данные будут считаны Процессом 2.
```cpp
write(fd1[1], buffer, strlen(buffer) + 1);
printf("Process 1 sent data: %s\n", buffer);
```

Когда Процесс 1 обратно берет работу на себя, он считывает обработанные данные из канала `fd2` с помощью `read` и помещает их в буфер.
```cpp
int num_bytes_read = read(fd2[0], buffer, sizeof(buffer));
if (num_bytes_read < 0) {
    perror("read");
    exit(EXIT_FAILURE);
}
printf("Process 1 received data: %s\n", buffer);
```

После этого Процесс 1 записывает данные в указанный выходной файл `output_file`
```cpp
FILE *out_fp = fopen(output_file, "w");
if (out_fp == NULL) {
    perror("open output file");
    exit(EXIT_FAILURE);
}
fprintf(out_fp, "%s", buffer);
fclose(out_fp);
```

Вначале своей работы Процесс 2 читает данные, которые были отправлены процессом 1 через канал `fd1`
```cpp
int num_bytes_read = read(fd1[0], buffer, sizeof(buffer));
if (num_bytes_read < 0) {
    perror("read");
    exit(EXIT_FAILURE);
}
printf("Process 2 received data: %s\n", buffer);
```

Далее Процесс 2 производит обработку данных: вызов функции findSequence
```cpp
findSequence(buffer, n, buffer);
if (buffer[0] == '\0') {
    strcpy(buffer, "No such sequence found.");
}
```

Измененный буфер отправляется Процессу 1 для записи результата в выходной файл:
```cpp
write(fd2[1], buffer, strlen(buffer) + 1);
printf("Process 2 sent data: %s\n", buffer);
```

## Скриншоты демонстрирующие работу приложения через Терминал

### Пример 1. Некорректный ввод
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/9c6662ef-7303-4ea9-8d8c-60f7e053a104)

### Пример 2 

#### Содержимое файла input1.txt 
<img width="649" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/a09ae7ba-a4d6-411e-9391-2c4311141828">

#### Результат в терминале
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/1e9fe4da-7979-4124-b0a5-e68a9a030bbc)

#### Содержимое output1.txt
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/52e7205b-7dec-4e92-a73e-b934584b639c)

### Пример 3. N > длины строки в файле

#### Содержимое файла input2.txt 
<img width="648" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/40719d22-182c-440a-bcb9-2647a0578e4b">

#### Результат в терминале
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/4e709d33-c9b9-40df-8f51-aade213bb860)

#### Содержимое output2.txt
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/8e0017c5-fde8-4280-8e88-f4d2b6574f7a)

### Пример 4. Нет нужной последовательности

#### Содержимое файла input3.txt 
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/8f4a630c-f2a5-465d-8242-52411ba7f7a9)

#### Результат в терминале
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/a65fbbf9-ae84-425e-8031-a92e44faa498)

#### Содержимое output3.txt
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/c2d6fa03-b114-43e8-9772-9f610f4d8737)

### Пример 5. 1000 символов

#### Содержимое файла input4.txt 
<img width="649" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/c9e82c7c-2610-4814-8cde-df2514aac9ff">

#### Результат в терминале
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/2e612652-2a85-4fdb-a972-ef9f464ccc53)

#### Содержимое output4.txt
<img width="649" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/e9d334d2-ae51-43cb-ab3b-97108cae8651">

### Пример 6. 5000 символов

#### Содержимое файла input5.txt 
<img width="1083" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/e9ce4b24-fc1e-48fd-a19a-6e41d34c4fc5">

#### Результат в терминале
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/36a4f689-3786-4f30-bbff-926522af12a3)

![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/35e5dc23-feec-48c2-b96a-951a2eb39639)

#### Содержимое output5.txt
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/d80b3582-81a2-4e77-a512-0edb263a9088)
