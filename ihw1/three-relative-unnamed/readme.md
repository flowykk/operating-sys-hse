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
