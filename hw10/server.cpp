#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define MAXPENDING 5
#define BUFFSIZE 32

void DieWithError(const char *mess) {
    perror(mess);
    exit(1);
}

int main(int argc, char *argv[]) {
    int serversock, clientsock1, clientsock2;
    struct sockaddr_in echoServAddr, echoClient;
    char buffer[BUFFSIZE];
    unsigned int recvMsgSize;
    int totalBytesRcvd;

    if ((argc < 3) || (argc > 4)) {
        fprintf(stderr, "Usage: %s <Server IP> <Server Port>\n", argv[0]);
        exit(1);
    }

    if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
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

    if (listen(serversock, MAXPENDING) < 0) {
        DieWithError("Failed to listen on server socket");
    }
    printf("Server IP address = %s. Wait...\n", inet_ntoa(echoServAddr.sin_addr));

    for(;;) {
        recvMsgSize = sizeof(echoClient);

        if ((clientsock1 = accept(serversock, (struct sockaddr *) &echoClient, &recvMsgSize)) < 0) {
            DieWithError("Failed to accept client connection");
        } else {
            fprintf(stdout, "Client connected: %s\n", inet_ntoa(echoClient.sin_addr));
        }

        if ((clientsock2 = accept(serversock, (struct sockaddr *) &echoClient, &recvMsgSize)) < 0) {
            DieWithError("Failed to accept client connection");
        } else {
            fprintf(stdout, "Client2 connected: %s\n", inet_ntoa(echoClient.sin_addr));
        }

        if ((totalBytesRcvd = recv(clientsock1, buffer, BUFFSIZE, 0)) < 0) {
            DieWithError("Failed to receive initial bytes from client");
        }
        buffer[totalBytesRcvd] = '\0';

        if (strcmp(buffer, "clear\n") == 0) {
            break;
        }
        bool flag = false;

        while (totalBytesRcvd > 0) {
            if (send(clientsock2, buffer, totalBytesRcvd, 0) != totalBytesRcvd) {
                DieWithError("Failed to send bytes to client");
            } else {
                fprintf(stdout, "Handling server\n");
            }
            if (strcmp(buffer, "clear\n") == 0) {
                flag = true;
                break;
            }
            if ((totalBytesRcvd = recv(clientsock1, buffer, BUFFSIZE, 0)) < 0) {
                DieWithError("Failed to receive additional bytes from client");
            }
            buffer[totalBytesRcvd] = '\0';

        }
        if (flag) {
            break;
        }

    }
    close(clientsock1);
    close(clientsock2);
    return 0;
}
