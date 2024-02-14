# ДЗ №4

__Работу выполнил__: Рахманов Данила Дмитриевич

## Что сделано в ДЗ №4?

### Оценка 10:
- Разработана программа, читающая файл и записывающая его содержимое в другой файл, используюшая только системные вызовы.
- Имена файлов для чтения и записи задаваются с использованием аргументов командной строки.
- Вместо большого буфера используется для работы с файлами буфер ограниченного размера.
- Читаются и переписываются не только текстовые, но и исполняемые файлы (текстовые и бинарные, содержащие признак исполняемости), а права доступа сохраняются.

## Код программы
### [Главный файл программы](main.cpp)

## Скриншоты, демонстрирующие работу программы

### Создание копии текстового файла

#### Изначальное содержимое text.txt
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/a0976234-cf63-4d00-9701-10bda078a53a">

#### Действия в терминале
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/b9266e07-cfe8-4568-b78f-5ba1202fde6a">

#### Содержимое textcopy.txt
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/ac3c730f-abe2-4922-ac6e-695c811d50c8">

### Создание копии бинарного файла

#### Изначальное содержимое binary.jpeg
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/ca4f2994-1e66-4baf-90c0-c080e6fe30c8">

#### Действия в терминале
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/1818897a-d13c-4a92-aa50-242de5253a54">

#### Содержимое binarycopy.jpeg
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/10614e80-dd00-485a-8161-e47ee2f610b4">

### Создание копии бинарного файла

#### Изначальное содержимое main.cpp
[Главный файл программы](main.cpp)

#### Действия в терминале
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/b32973f1-bbea-4298-8ada-232fb10fa5a7">

#### Содержимое maincopy.cpp
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/a56135ea-2216-4310-abfe-bfe285d1f62b">
