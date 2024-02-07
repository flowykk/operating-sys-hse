# ДЗ №3

__Работу выполнил__: Рахманов Данила Дмитриевич

## Что сделано в ДЗ №3?

### Оценка 10:
- Разработана программа, в которой родительский процесс вычисляет число Фибоначчи, а процесс-ребенок вычисляет значение факториала
- Программа обрабатывает возникающие переполнения
- Для всех процессов выводится на экран дополнительная информация об их потомках и родителях
- По завершении вычислений выводится информация о содержимом текущего каталога

## Код программы
### [Главный файл программы](main.cpp)

## Лог программы
```
danilarahmanov@MacBook-Pro-Danila untitled % ./a.out
Input the number: 0 
Number must be >0 !
Input the number: -1
Number must be >0 !
Input the number: -100
Number must be >0 !
Input the number: 1
I am a parent! My pid = 91803, my parent is 90992, my child is 91804 and factorial is 1.
I am a child! My pid = 91804, my parent is 91803 and fibonacci is 1.

Current catalog info:
Content of the current working directory: 
CMakeLists.txt		cmake-build-debug
a.out			main.cpp

danilarahmanov@MacBook-Pro-Danila untitled % ./a.out
Input the number: 50
I am a parent! My pid = 91819, my parent is 90992, my child is 91820, but OVERFLOW detected computing factorial!
I am a child! My pid = 91820, my parent is 91819, but OVERFLOW detected computing fibonacci!

Current catalog info:
Content of the current working directory: 
CMakeLists.txt		cmake-build-debug
a.out			main.cpp

danilarahmanov@MacBook-Pro-Danila untitled % 20
zsh: command not found: 20
danilarahmanov@MacBook-Pro-Danila untitled % ./a.out
Input the number: 20
I am a parent! My pid = 91829, my parent is 90992, my child is 91830, but OVERFLOW detected computing factorial!
I am a child! My pid = 91830, my parent is 91829 and fibonacci is 6765.

Current catalog info:
Content of the current working directory: 
CMakeLists.txt		cmake-build-debug
a.out			main.cpp

danilarahmanov@MacBook-Pro-Danila untitled % ./a.out
Input the number: 5
I am a parent! My pid = 91834, my parent is 90992, my child is 91835 and factorial is 120.
I am a child! My pid = 91835, my parent is 91834 and fibonacci is 5.

Current catalog info:
Content of the current working directory: 
CMakeLists.txt		cmake-build-debug
a.out			main.cpp
```

## Скриншоты, демонстрирующие работу программы

### Пример проверки некорректности входных данных
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/831e294b-768d-42e4-a60d-4ed6865765c6">

### Примеры работы программы без переполнений
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/b16551fa-8fa6-4024-a50c-ce5ec371715a">
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/9e6f6d84-f04a-4ffd-9e6e-5a77e1444c3c">

### Примеры возникновения переполнения
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/0071e8d1-5f9b-47cb-8f7c-20026be0a361">
<img width="850" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/5f5fb239-6d86-4cb4-8786-89bd7065ceb0">

