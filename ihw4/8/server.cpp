#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>

#define MAXPENDING 5
#define BUFFSIZE 32

void DieWithError(const char *mess) {
    perror(mess);
    exit(1);
}

int main(int argc, char *argv[]) {
    int serversock;
    std::vector<struct sockaddr_in> clients;
    struct sockaddr_in echoServAddr, echoClient;
    char buffer[BUFFSIZE];
    unsigned int recvMsgSize;
    socklen_t clientLen;
    int totalBytesRcvd;
    int messagesReceived = 0;

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
    } else {
        fprintf(stdout, "Server socket binded\n");
    }

    printf("Server IP address = %s. Wait...\n", inet_ntoa(echoServAddr.sin_addr));

    clientLen = sizeof(echoClient);

    if ((totalBytesRcvd = recvfrom(serversock, buffer, BUFFSIZE, 0, (struct sockaddr *) &echoClient, &clientLen)) < 0) {
        DieWithError("Failed to receive initial bytes from client");
    }
    buffer[totalBytesRcvd] = '\0';
    int numberOfClients = std::atoi(buffer);

    clients.push_back(echoClient);
    for (int i = 1; i < numberOfClients; i++) {
        struct sockaddr_in newClient;
        clientLen = sizeof(newClient);
        if ((totalBytesRcvd = recvfrom(serversock, buffer, BUFFSIZE, 0, (struct sockaddr *) &newClient, &clientLen)) < 0) {
            DieWithError("Failed to receive initial bytes from client");
        }
        clients.push_back(newClient);
    }

    while (true) {
        clientLen = sizeof(echoClient);
        if ((totalBytesRcvd = recvfrom(serversock, buffer, BUFFSIZE, 0, (struct sockaddr *) &echoClient, &clientLen)) < 0) {
            DieWithError("Failed to receive additional bytes from client");
        }
        buffer[totalBytesRcvd] = '\0';
        messagesReceived++;

        for (int i = 0; i < numberOfClients - 1; i++) {
            if (sendto(serversock, buffer, totalBytesRcvd, 0, (struct sockaddr *) &clients[i], sizeof(clients[i])) != totalBytesRcvd) {
                DieWithError("Failed to send bytes to client");
            } else {
                fprintf(stdout, "Handling server\n");
            }
        }

        if (messagesReceived >= 6) {
            break;
        }
    }

    close(serversock);

    return 0;
}
