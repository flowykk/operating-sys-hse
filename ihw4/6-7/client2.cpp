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
    int sock;
    struct sockaddr_in serverAddr, fromAddr;
    char buffer[BUFFSIZE];
    socklen_t fromSize = sizeof(fromAddr);
    int totalBytesRcvd;
    int messagesReceived = 0;

    if ((argc < 3) || (argc > 4)) {
        fprintf(stderr, "USAGE: client2 <server_ip> <port>\n");
        exit(1);
    }

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        DieWithError("Failed to create socket");
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_port = htons(atoi(argv[2]));

    printf("Waiting for the server to send data...\n");

    while (true) {
        if ((totalBytesRcvd = recvfrom(sock, buffer, BUFFSIZE - 1, 0, (struct sockaddr *) &fromAddr, &fromSize)) <= 0) {
            DieWithError("Failed to receive initial bytes from server");
        }

        buffer[totalBytesRcvd] = '\0';

        printf("Received and Send to Server3: %s", buffer);

        messagesReceived++;
        if (messagesReceived >= 6) {
            close(sock);
            break;
        }
    }

    return 0;
}
