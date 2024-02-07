#include <unistd.h>
#include <stdio.h>
#include <iostream>

int fibonacci(int n) {
    if (n <= 1)
        return n;

    int fib0 = 0;
    int fib1 = 1;
    int res;

    for (int i = 2; i <= n; ++i) {
        if (UINT64_MAX - fib1 < fib0) {
            return -1;
        }

        res = fib0 + fib1;
        fib0 = fib1;
        fib1 = res;
    }

    return res;
}

int factorial(int n) {
    int res = 1;
    for (int i = 1; i <= n; ++i) {
        if (UINT64_MAX / i < res) {
            return -1;
        }

        res *= i;
    }
    return res;
}

int main() {
    int num;

    while(true)
    {
        std::cout << "Input the number: ";
        std::cin >> num;

        if (num <= 0) {
            std::cout << "Number must be >0 !" << std::endl;
        } else {
            break;
        }
    }

    pid_t pid, ppid, chpid;
    chpid = fork();
    pid  = getpid();
    ppid = getppid();

    if (!chpid) {
        int fib_result = fibonacci(num);

        if (fib_result == -1) {
            printf("I am a child! My pid = %d, my parent is %d, but OVERFLOW detected computing fibonacci!\n", (int)pid, (int)ppid);
        } else {
            printf("I am a child! My pid = %d, my parent is %d and fibonacci is %d.\n", (int)pid, (int)ppid, (int)fib_result);
        }
    } else {
        int fact_result = factorial(num);

        if (fact_result == -1) {
            printf("I am a parent! My pid = %d, my parent is %d, my child is %d, but OVERFLOW detected computing factorial!\n", (int)pid, (int)ppid, (int)chpid);
        } else {
            printf("I am a parent! My pid = %d, my parent is %d, my child is %d and factorial is %d.\n", (int)pid, (int)ppid, (int)chpid, (int)fact_result);
        }
    }

    sleep(2);

    if (chpid) {
        std::cout << "\nCurrent catalog info:\n";
        std::cout << "Content of the current working directory: \n";
        system("ls");
    }

    return 0;
}
