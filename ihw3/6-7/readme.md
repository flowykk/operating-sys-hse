# Отчёт для программы: 6-7 баллов. Клиент–серверное приложение, в котором есть дополнительный клиент, предназначенный для отображение комплексной информации о выполнении приложения в целом.  

## Код программы

Клиент №1 - [client.cpp](client.cpp)

Клиент №2 - [client2.cpp](client2.cpp)

Клиент №3 - [client2.cpp](client3.cpp)

Сервер - [server.cpp](server.cpp)

## Описание входных данных

В терминале `Клиента №1` пользователю предлагается ввести энергию Ций каждого из бойцов.

## Описание алгоритма работы программы

- `Клиент №1` предлагает клиенту ввести энергию Ций каждого из бойцов
- `Сервер` получает введённые данные и перенаправляет эти данные `Клиенту №2`, а также выводит сообщение об этом
- `Клиент №2` получает данные от `Сервера` и выводит данные на экран, а также отправляет их `Клиенту 3` для симуляции соревнований
- `Клиент №3` получает эти данные и проводит симуляцию соревнований, используя помощи именнованные POSIX-семафоры
- `Клиент №3` завершает работу, когда заканчивается финальный бой
- Когда пользователь вводит последнее число, означающее энергию Ций, `Клиент №1`, `Клиент №2` и `Сервер` завершают свою работу

## Описание кода программы

`Клиент №2`, `Клиент №1` - идентичны коду на оценку [4-5](../4-5) 

`Клиент №3` - Клиент, где происходит основная логика приложения, а именно процесс проведения соревнования.

Для  удобства работы с данными бойца написана структура `Fighter`
```cpp
struct Fighter {
    int strength;
    bool defeated;

    explicit Fighter(int str) : strength(str), defeated(false) {}
};
```

Для проведения боя между бойцами написана функция `fight(..)`, а для проведения финального боя - функция `finalFight(..)`
```cpp
void fight(Fighter& fighter1, Fighter& fighter2, sem_t* semaphore) {
    std::this_thread::sleep_for(std::chrono::milliseconds (fighter2.strength / fighter1.strength * 1000));

    printf("Бой: Боец с силой %d против бойца с силой %d\n", fighter1.strength, fighter2.strength);
    if (fighter1.strength > fighter2.strength) {
        fighter1.strength += fighter2.strength;
        fighter2.defeated = true;
        printf("Победитель: Боец с силой %d; Новая сила: %d\n", fighter1.strength - fighter2.strength, fighter1.strength);
    } else {
        fighter2.strength += fighter1.strength;
        fighter1.defeated = true;
        printf("Победитель: Боец с силой %d; Новая сила: %d\n", fighter2.strength - fighter1.strength, fighter2.strength);
    }

    sem_post(semaphore);
}

void finalFight(const std::vector<Fighter>& fighters, sem_t* semaphore) {
    int winner1_idx = -1, winner2_idx = -1;
    for (int i = 0; i < numFighters; ++i) {
        if (!fighters[i].defeated) {
            if (winner1_idx == -1) {
                winner1_idx = i;
            } else {
                winner2_idx = i;
                break;
            }
        }
    }

    if (winner1_idx != -1 && winner2_idx != -1) {
        std::thread fight_thread(fight, std::ref(const_cast<Fighter&>(fighters[winner1_idx])), std::ref(const_cast<Fighter&>(fighters[winner2_idx])), semaphore);
        fight_thread.join();
        sem_wait(semaphore);
        std::cout << "Финальный бой: Боец " << winner1_idx + 1 << " против Бойца " << winner2_idx + 1 << std::endl;
    }
}
```

Для симуляции каждого из боев была написана функция `sim(..)`, которая использует семафоры для управления разными боями
```cpp
void sim(std::vector<Fighter> fighters) {
    sem_t* semaphore = sem_open(SEMAPHORE_NAME, O_CREAT, 0666, 0);
    if (semaphore == SEM_FAILED) {
        std::cerr << "Ошибка создания семафора." << std::endl;
        return;
    }

    std::thread fights[3];
    for (int i = 0; i < 3; ++i) {
        fights[i] = std::thread(fight, std::ref(fighters[i * 2]), std::ref(fighters[i * 2 + 1]), semaphore);
    }

    for (int i = 0; i < 3; ++i) {
        sem_wait(semaphore);
    }

    for (int i = 0; i < 3; ++i) {
        fights[i].join();
    }

    finalFight(fighters, semaphore);

    int winner_strength = -1;
    for (const auto& fighter : fighters) {
        if (!fighter.defeated) {
            winner_strength = fighter.strength;
            break;
        }
    }
    if (winner_strength != -1) {
        std::cout << "Сила победителя соревнования: " << winner_strength << std::endl;
    } else {
        std::cout << "Все бойцы были побеждены." << std::endl;
    }

    sem_close(semaphore);
    sem_unlink(SEMAPHORE_NAME);
}
```

## !!!

Все объяснения работы функций `fight(..)`, `finalFight(..)`, работа с семафорами и т.д. были подробно приведены в [ИДЗ №2](../../ihw2)

## Скриншоты, демонстрирующие работу программы

### Действия клиента №1

<img width="726" alt="Снимок экрана 2024-05-18 в 21 47 14" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/4fb79c7b-69e2-42be-85c6-0b1b2b542337">

### Действия сервера

<img width="724" alt="Снимок экрана 2024-05-18 в 21 47 21" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/761f8c7c-5f47-4e58-b4bf-79177de7bf6c">

### Действия клиента №2

<img width="728" alt="Снимок экрана 2024-05-18 в 21 47 28" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/00afc1c8-b544-4f25-af84-7c4bec1942a2">

### Действия клиента №3

<img width="727" alt="Снимок экрана 2024-05-18 в 21 47 36" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/9770edd7-2c9b-4d8b-971c-11e30bf9d45f">
