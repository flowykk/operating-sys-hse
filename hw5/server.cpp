#include <iostream>
#include <unistd.h>
#include <csignal>
#include <vector>
#include <algorithm>

bool flag = false;
std::vector<int> result;

void buildNumberHandler(int sign) {
    if (sign == SIGUSR2) {
        result.push_back(1);
    } else {
        result.push_back(0);
    }
}

void terminateHandler(int sign) {
    flag = true;
}

int main() {
    result.resize(32);
    signal(SIGUSR1, buildNumberHandler);
    signal(SIGUSR2, buildNumberHandler);
    signal(SIGTERM, terminateHandler);

    pid_t pid = getpid();
    std::cout << "RECEIVER pid " << pid;

    pid_t sender_pid;
    std::cout << "\nEnter SENDER id: ";
    std::cin >> sender_pid;

    while (!flag) {
        kill(sender_pid, SIGUSR1);
        pause();
    }

    std::reverse(result.begin(), result.end());
    std::cout << "Received number's bits are ";
    for (int i = 0; i < 32; ++i) {
        std::cout << result[i];
    }

    int mult = 1;
    int ans = 0;
    int mask = 0;
    if (result[0] == 1) mask = 1;

    for (int i = 31; i > 0; --i) {
        ans += (result[i] ^ mask) * mult;
        mult *= 2;
    }

    std::cout << "\nInitial number was " << ans << std::endl;

    return 0;
}
