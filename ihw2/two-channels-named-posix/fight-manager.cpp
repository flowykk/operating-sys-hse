#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

const int numFighters = 6;
const char* FIGHT_CHANNEL = "/fight_channel";

struct Fighter {
    int strength;
    bool defeated;

    explicit Fighter(int str) : strength(str), defeated(false) {}
};

void fight(Fighter& fighter1, Fighter& fighter2, int fight_channel) {
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
    write(fight_channel, "1", 1);
}

int main() {
    mkfifo(FIGHT_CHANNEL, 0666);

    int fight_channel = open(FIGHT_CHANNEL, O_WRONLY);
    if (fight_channel == -1) {
        std::cerr << "Ошибка открытия канала для боев." << std::endl;
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    std::vector<Fighter> fighters;
    for (int i = 0; i < numFighters; ++i) {
        int strength = dis(gen);
        fighters.emplace_back(strength);
    }

    for (int i = 0; i < numFighters; ++i) {
        printf("Боец %d: %d\n", i + 1, fighters[i].strength);
    }

    fight(fighters[0], fighters[1], fight_channel);
    fight(fighters[2], fighters[3], fight_channel);
    fight(fighters[4], fighters[5], fight_channel);

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

    close(fight_channel);
    unlink(FIGHT_CHANNEL);

    return 0;
}
