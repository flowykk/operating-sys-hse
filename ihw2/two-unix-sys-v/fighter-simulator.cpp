#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <vector>

// Количество бойцов
const int numFighters = 6;
// Имя именованного семафора
const char* SEMAPHORE_NAME = "/fighter_semaphore";

int main() {
    // Создание разделяемой памяти для хранения данных о бойцах
    key_t shm_key = ftok(".", 's');
    int shmid = shmget(shm_key, sizeof(int) * numFighters, 0666 | IPC_CREAT);

    // Получение доступа к разделяемой памяти
    int* fighter_strengths = (int*)shmat(shmid, NULL, 0);

    // Создание и инициализация массива бойцов
    std::vector<int> fighters;
    for (int i = 0; i < numFighters; ++i) {
        int strength;
        std::cout << "Введите энергию Ций бойца " << i + 1 << ": ";
        std::cin >> strength;
        fighters.push_back(strength);
    }

    // Копирование данных в разделяемую память
    for (int i = 0; i < numFighters; ++i) {
        fighter_strengths[i] = fighters[i];
    }

    // Создание семафора для управления доступом к разделяемой памяти
    int semid = semget(shm_key, 1, 0666 | IPC_CREAT);
    semctl(semid, 0, SETVAL, 1);

    // Засыпаем, чтобы дать временной на первую программу прочитать данные
    sleep(3);

    // Освобождение ресурсов
    shmdt(fighter_strengths);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
