#include "echoServer.h"
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>

#define MAXPENDING 5
#define BUFFSIZE 32
#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(const char *mess) {
    perror(mess);
    exit(1);
}

void HandleTCPClient(int clntSocket, int clientsock1)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int recvMsgSize;                    /* Size of received message */

    /* Receive message from client */
    if ((recvMsgSize = recv(clientsock1, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        /* Echo message back to client */
        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");
    }

    close(clntSocket);    /* Close client socket */
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
    pid_t processID;
    std::vector<int> clntSocks;

    struct sockaddr_in echoServAddr, echoClient;
    char buffer[BUFFSIZE];
    unsigned int recvMsgSize;
    int totalBytesRcvd;
    int messegesReceived = 0;
    int clientsConntected = 0;

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
        int clntSock;
        clientsConntected++;
        if (clientsConntected == 1) {
            clientsock1 = AcceptTCPClient(serversock);
        } else {
            clntSock = AcceptTCPClient(serversock);
            if ((processID = fork()) < 0)
                DieWithError("fork() failed");
            else if (processID == 0)  /* If this is the child process */
            {
                close(serversock);   /* Child closes parent socket */

                if ((totalBytesRcvd = recv(clientsock1, buffer, BUFFSIZE, 0)) < 0) {
                    DieWithError("Failed to receive initial bytes from client");
                }
                buffer[totalBytesRcvd] = '\0';
                bool flag = false;

                while (totalBytesRcvd > 0) {
                    messegesReceived++;
                    if (send(clntSock, buffer, totalBytesRcvd, 0) != totalBytesRcvd)
                    {
                        DieWithError("send() failed");
                    } else {
                        fprintf(stdout, "Handling server\n");
                    }

                    if (messegesReceived >= 6) {
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
                //HandleTCPClient(clntSock, clientsock1);

                exit(0);           /* Child process terminates */
            }
        }

    }

    close(clientsock1);
    close(clientsock2);
    return 0;
}
