# Отчёт для программы: Взаимодействие между двумя родственными процессами через именованные каналы в двух разных программах

## Код программы

- Код процесса 1 - [unrelative-process-1.cpp](unrelative-process-1.cpp)
- Код процесса 2 - [unrelative-process-2.cpp](unrelative-process-2.cpp)

## Тестовые данные программы

Тестовые данные находятся в папке `../ihw1/Data/`

## Описание входных данных

В аргументах командной строки пользователю необходимо указать следующие данные:
- `-n` - Число N, необходимое для решения данной в варианте задачи. Если пользователь не укажет этот аргумент, или укажет n <= 0, то будет использовано значение по умолчанию
- `-in` - Путь к файлу с изначальными данными
- `-out` - Путь к файлу, куда надо записать результат работы

## Описание работы программы

## Скриншоты демонстрирующие работу программ

Считывание данных из терминала 
```cpp
for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-in") == 0) {
        input_file = argv[++i];
    } else if (strcmp(argv[i], "-out") == 0) {
        output_file = argv[++i];
    }
}
```

Открытие входного файла и обработка возможных ошибок
```cpp
int fd = open(input_file, O_RDONLY);
if (fd < 0) {
    printf("Can\'t open file for reading\n");
    exit(-1);
}
read(fd, buffer, BUFFER_SIZE);
close(fd);
```

Отправка данных Процессу 2 через канал `fifo1`
```cpp
fd = open(fifo1, O_WRONLY);
write(fd, buffer, BUFFER_SIZE);
close(fd);
printf("Process 1 sent data: %s\n", buffer);
```

Принятие данных от Процесса 2 через канал `fifo2` и запись полученных данных в выходной файл
```cpp
fd = open(fifo2, O_RDONLY);
read(fd, buffer, sizeof(buffer));
close(fd);
printf("Process 1 received data: %s\n", buffer);

FILE *out_fp = fopen(output_file, "w");
if (out_fp == NULL) {
    perror("open output file");
    exit(EXIT_FAILURE);
}
fprintf(out_fp, "%s", buffer);
fclose(out_fp);
```

В свою очерель Процесс 2 получает данные от Процесса 1 по каналу `fifo1` и обрабатывает их
```cpp
int fd = open(fifo1, O_RDONLY);
read(fd, buffer, BUFFER_SIZE);
close(fd);

findSequence(buffer, n, buffer);
```

После обработки Процесс 2 отправляет данные обратно по каналу `fifo2`
```cpp
fd = open(fifo2, O_WRONLY);
write(fd, buffer, sizeof(buffer));
close(fd);
```

### Пример 1

#### Результат в 1-ом окне терминала

![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/aa8fa151-352c-4605-9af2-d3c138a23404)

![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/1db86df4-4e94-43c1-8361-6bb94720b900)

#### Результат во 2-ом окне терминала
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/9a741203-5565-4f25-9dbe-0f929498c27f)

### Пример 2

#### Результат в 1-ом окне терминала
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/79f31470-9976-465d-857f-ea2f2ede4599)

#### Результат во 2-ом окне терминала
![image](https://github.com/flowykk/operating-sys-hse/assets/71427624/c11f7499-e05c-43bd-a346-15c7fedeeeba)



