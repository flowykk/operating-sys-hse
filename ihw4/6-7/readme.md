# Отчёт для программы: 6-7 баллов. Клиент–серверное приложение, в котором есть дополнительный клиент, предназначенный для отображение комплексной информации о выполнении приложения в целом.  

## Код программы

Клиент №1 - [client.cpp](client.cpp)

Клиент №2 - [client2.cpp](client2.cpp)

Клиент №3 - [client3.cpp](client3.cpp)

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

`Клиент №1` из программы на оценку [4-5](../4-5) был разделён на разные клиенты: `Клиент №1` и `Клиент №2`, но код остаётся тем же.

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

<img width="730" alt="Снимок экрана 2024-06-01 в 19 08 07" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/18f718a5-8ae8-4b70-9984-d36faf89bd6c">

### Действия сервера

<img width="731" alt="image" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/13f2384a-04b4-4de6-ab46-6b34d2e672c9">

### Действия клиента №2

<img width="731" alt="Снимок экрана 2024-06-01 в 19 08 23" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/d654b6ec-b57f-4ab9-a834-0f5a430d3658">

### Действия клиента №3

<img width="731" alt="Снимок экрана 2024-06-01 в 19 08 35" src="https://github.com/flowykk/operating-sys-hse/assets/71427624/c1279710-f1db-4c0f-9d0c-27f379723a81">
