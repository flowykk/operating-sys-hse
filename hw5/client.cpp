#include <iostream>
#include <csignal>
#include <unistd.h>

void closure(int sign) {
}

int main() {
    signal(SIGUSR1, closure);

    pid_t pid = getpid();
    std::cout << "SENDER pid " << pid << std::endl;

    pid_t receiver_pid;
    std::cout << "Enter RECEIVER Id: ";
    std::cin >> receiver_pid;

    int num;
    std::cout << "Enter Initial number: ";
    std::cin >> num;
    std::cout << "\n";

    for (int i = 0; i < 32; ++i) {
        int bit = num & 1;
        if (!bit) {
            kill(receiver_pid, SIGUSR1);
        } else {
            kill(receiver_pid, SIGUSR2);
        }
        num >>= 1;
        pause();
    }

    kill(receiver_pid, SIGTERM);
    return 0;
}
