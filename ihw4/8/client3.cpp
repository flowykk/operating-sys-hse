#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <semaphore.h>
#include <random>
#include <chrono>
#include <mutex>
#include <queue>
#include <iostream>

using namespace std;

#define BUFFSIZE 32

const int numFighters = 6;
const char* SEMAPHORE_NAME = "/fighter_semaphore";

struct Fighter {
    int strength;
    bool defeated;

    explicit Fighter(int str) : strength(str), defeated(false) {}
};

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

void DieWithError(const char *mess) {
    perror(mess);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in echoServAddr, fromAddr;
    char buffer[BUFFSIZE];
    socklen_t fromSize = sizeof(fromAddr);
    int totalBytesRcvd;
    int messagesReceived = 0;
    std::vector<Fighter> fighters;

    if ((argc < 3) || (argc > 4)) {
        fprintf(stderr, "USAGE: client3 <server_ip> <port>\n");
        exit(1);
    }

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        DieWithError("Failed to create socket");
    }

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(argv[1]);
    echoServAddr.sin_port = htons(atoi(argv[2]));

    printf("Waiting for the server to send data...\n");

    while (true) {
        if ((totalBytesRcvd = recvfrom(sock, buffer, BUFFSIZE - 1, 0, (struct sockaddr *) &fromAddr, &fromSize)) <= 0) {
            DieWithError("Failed to receive initial bytes from server");
        }

        buffer[totalBytesRcvd] = '\0';

        int energy = std::atoi(buffer);
        fighters.emplace_back(energy);

        messagesReceived++;
        if (messagesReceived >= 6) {
            close(sock);
            sim(fighters);
            break;
        }
    }

    return 0;
}
