#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define BUFFSIZE 32

void DieWithError(const char *mess) {
    perror(mess);
    exit(1);
}

int main(int argc, char *argv[]) {
    int serversock;
    struct sockaddr_in echoServAddr, echoClntAddr;
    char buffer[BUFFSIZE];
    int recvMsgSize;
    unsigned int clntAddrLen = sizeof(echoClntAddr);
    int messagesReceived = 0;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server IP> <Server Port>\n", argv[0]);
        exit(1);
    }

    if ((serversock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        DieWithError("Failed to create socket");
    }

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(argv[1]);
    echoServAddr.sin_port = htons(atoi(argv[2]));

    if (bind(serversock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
        DieWithError("Failed to bind the server socket");
    } else {
        fprintf(stdout, "Server socket binded\n");
    }

    while (true) {
        if ((recvMsgSize = recvfrom(serversock, buffer, BUFFSIZE - 1, 0, (struct sockaddr *)&echoClntAddr, &clntAddrLen)) < 0) {
            DieWithError("Failed to receive bytes from client");
        }

        buffer[recvMsgSize] = '\0'; // Null-terminate the received data
        printf("Received: %s", buffer);
        messagesReceived++;

        if (messagesReceived >= 6) {
            break;
        }
    }

    close(serversock);
    return 0;
}
