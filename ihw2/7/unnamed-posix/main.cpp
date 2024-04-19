#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <semaphore.h>

// Количество бойцов
const int numFighters = 6;

// Структура для хранения информации о бойце
struct Fighter {
    int strength;
    bool defeated;

    explicit Fighter(int str) : strength(str), defeated(false) {}
};

// Функция для симуляции боя между двумя бойцами
void fight(Fighter& fighter1, Fighter& fighter2, sem_t& semaphore) {
    // Симуляция боя
    std::this_thread::sleep_for(std::chrono::milliseconds (fighter2.strength / fighter1.strength * 1000));

    // Определение победителя и обновление силы
    sem_wait(&semaphore);
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
    sem_post(&semaphore);
}

// Функция для боя между победителями первых трех боев и их финальным боем
void finalFight(std::vector<Fighter>& fighters, sem_t& semaphore) {
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
        // Финальный бой между победителями первых трех боев
        fight(fighters[winner1_idx], fighters[winner2_idx], semaphore);

        // Бой между победителями предпоследнего и последнего боев
        for (auto& fighter : fighters) {
            if (!fighter.defeated && &fighter != &fighters[winner1_idx] && &fighter != &fighters[winner2_idx]) {
                fight(fighters[winner1_idx], fighter, semaphore);
                break;
            }
        }
    }
}


int main() {
    // Создание неименованного семафора
    sem_t semaphore;
    sem_init(&semaphore, 0, 1);

    // Инициализация генератора случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    // Создание и инициализация массива бойцов
    std::vector<Fighter> fighters;
    for (int i = 0; i < numFighters; ++i) {
        fighters.emplace_back(dis(gen));
    }

    printf("\nИнформация об энергиях Ций бойцов:\n");
    for (int i = 0; i < numFighters; ++i) {
        printf("Боец %d: %d\n", i + 1, fighters[i].strength);
    }
    printf("\n");

    // Начало битвы
    std::thread fights[3];
    for (int i = 0; i < 3; ++i) {
        fights[i] = std::thread(fight, std::ref(fighters[i * 2]), std::ref(fighters[i * 2 + 1]), std::ref(semaphore));
    }

    // Ожидание завершения боев
    for (int i = 0; i < 3; ++i) {
        fights[i].join();
    }

    finalFight(fighters, semaphore);

    // Вывод силы победителя соревнования
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

    // Уничтожение семафора
    sem_destroy(&semaphore);

    return 0;
}
