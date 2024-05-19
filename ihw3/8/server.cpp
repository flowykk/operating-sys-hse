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

int AcceptTCPClient(int servSock) {
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int clntLen;            /* Length of client address data structure */

    /* Set the size of the in-out parameter */
    clntLen = sizeof(echoClntAddr);

    /* Wait for a client to connect */
    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
        DieWithError("accept() failed");

    /* clntSock is connected to a client! */

    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

    return clntSock;
}

int main(int argc, char *argv[]) {
    int serversock;
    int clientsock1, clientsock2, clientsock3;
    std::vector<int> clients;
    struct sockaddr_in echoServAddr, echoClient;
    char buffer[BUFFSIZE];
    unsigned int recvMsgSize;
    int totalBytesRcvd;
    int messegesReceived = 0;

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

        clientsock1 = AcceptTCPClient(serversock);
        if ((totalBytesRcvd = recv(clientsock1, buffer, BUFFSIZE, 0)) < 0) {
            DieWithError("Failed to receive initial bytes from client");
        }
        buffer[totalBytesRcvd] = '\0';
        bool flag = false;

        int numberOfClients = std::atoi(buffer);
        clients.resize(numberOfClients - 1, 0);

        for (int i = 0; i < numberOfClients - 1; i++) {
            clients[i] = AcceptTCPClient(serversock);
        }

        while (totalBytesRcvd > 0) {
            messegesReceived++;
            if ((totalBytesRcvd = recv(clientsock1, buffer, BUFFSIZE, 0)) < 0) {
                DieWithError("Failed to receive additional bytes from client");
            }
            buffer[totalBytesRcvd] = '\0';

            for (int i = 0; i < numberOfClients - 1; i++) {
                if (send(clients[i], buffer, totalBytesRcvd, 0) != totalBytesRcvd) {
                    DieWithError("Failed to send bytes to client");
                } else {
                    fprintf(stdout, "Handling server\n");
                }
            }

            if (messegesReceived >= 6) {
                flag = true;
                break;
            }
        }

        if (flag) {
            break;
        }

    }

    close(clientsock1);
    for (int i = 0; i < clients.size(); i++) {
        close(clients[i]);
    }

    return 0;
}
