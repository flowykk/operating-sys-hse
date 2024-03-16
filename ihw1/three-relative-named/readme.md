# Отчёт для программы: Взаимодействие между тремя родственными процессами через именованные каналы

## В отчёте присутствуют:
- [Код программы]()
- [Тестовые данные программые]()
- [Описание входных данных]()
- [Описание работы программы]()
- [Скриншоты демонстрирующие работу приложения через Терминал]()

## Код программы

Файл программы - [three-relative-named.cpp](three-relative-named.cpp)

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

Далее происходит создание двух каналов и обработка ошибок.
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
    pid2 = fork();
    if (pid2 < 0) {
        // fail
    } else if (pid2 == 0) {
        // work
    } else { // Процесс 3
        // work
    }
}
```

Следующий фрагмент кода отвечает за то, чтобы Процесс 1 открыл файл `FIFO_FILE_1` канал `fifo1` для записи и отправки данных другому процессу 
```cpp
else if (pid1 == 0) {
    // work
    fd1 = open(FIFO_FILE_1, O_WRONLY);
    // work
}
```

Фрагмент кода ниже выполняет похожую процедуру: Процесс 2 открывает `FIFO_FILE_1` для чтения и считывает данные из него.
```cpp
} else if (pid2 == 0) {
    // work
    fd1 = open(FIFO_FILE_1, O_RDONLY);
    // work
}
```

Далее Процесс 3 открывает `FIFO_FILE_2` для чтения и считывает данные из файла.
```cpp
} else { 
    // work
    fd = open(FIFO_FILE_2, O_RDONLY);
    // work
}
```

После этого происходит запись результата в указанный файл и освобождение именованных каналов.
```cpp
FILE *fp = fopen(output_file, "w");
if (fp == NULL) {
    perror("open output file");
    exit(-1);
}
fprintf(fp, "%s", buffer);
fclose(fp);

// Удаление FIFOs
if (unlink(FIFO_FILE_1) == -1) {
    perror("unlink FIFO_FILE_1");
    exit(-1);
}
if (unlink(FIFO_FILE_2) == -1) {
    perror("unlink FIFO_FILE_2");
    exit(-1);
}
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

### Пример 4 

#### Ввод в терминале 

#### Содержимое файла input3.txt 

#### Результат в терминале

#### Содержимое output3.txt

### Пример 5 

#### Ввод в терминале 

#### Содержимое файла input4.txt 

#### Результат в терминале

#### Содержимое output4.txt

### Пример 6

#### Ввод в терминале 

#### Содержимое файла input5.txt 

#### Результат в терминале

#### Содержимое output5.txt
