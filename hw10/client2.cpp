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
    struct sockaddr_in echoServAddr;
    char buffer[BUFFSIZE];
    int totalBytesRcvd;

    if ((argc < 3) || (argc > 4)) {
        fprintf(stderr, "USAGE: client <server_ip> <port>\n");
        exit(1);
    }

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        DieWithError("Failed to create socket");
    }

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(argv[1]);
    echoServAddr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
        DieWithError("Failed to connect with server");
    }
    printf("Waiting for the server to send data...\n");

    for(;;) {
        // Receive data from the server
        if ((totalBytesRcvd = recv(sock, buffer, BUFFSIZE - 1, 0)) <= 0) {
            DieWithError("Failed to receive initial bytes from server");
        }

        // Null-terminate the totalBytesRcvd data
        buffer[totalBytesRcvd] = '\0';

        // Print out the totalBytesRcvd data
        printf("Received: %s", buffer);

        if (strcmp(buffer, "clear\n") == 0) {
            close(sock);
            break;
        }
    }

    return 0;
}
