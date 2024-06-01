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
    struct sockaddr_in echoServAddr, clientAddr;
    char buffer[BUFFSIZE];
    socklen_t cliAddrLen;
    int messegesReceived = 0;

    if ((argc < 3) || (argc > 4)) {
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
    }

    cliAddrLen = sizeof(clientAddr);
    printf("Server IP address = %s. Wait...\n", inet_ntoa(echoServAddr.sin_addr));

    for(;;) {
        if (recvfrom(serversock, buffer, BUFFSIZE - 1, 0, (struct sockaddr *) &clientAddr, &cliAddrLen) < 0) {
            DieWithError("Failed to receive initial bytes from client");
        }

        buffer[BUFFSIZE - 1] = '\0';
        messegesReceived++;

        if (sendto(serversock, buffer, strlen(buffer), 0, (struct sockaddr *) &clientAddr, cliAddrLen) != strlen(buffer)) {
            DieWithError("Failed to send bytes to client");
        }

        if (messegesReceived >= 6) {
            break;
        }
    }

    close(serversock);
    return 0;
}
