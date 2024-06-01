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
    unsigned int recvMsgSize;

    if ((argc < 3) || (argc > 4)) {
        fprintf(stderr, "Usage: %s <Server IP> <Server Port>\n", argv[0]);
        exit(1);
    }

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        DieWithError("Failed to create socket");
    }

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(argv[1]);
    echoServAddr.sin_port = htons(atoi(argv[2]));

    int n = 7;
    for (int i = 0; i < n; i++) {
        if (i == 0)
            printf("Enter number of future clients: ");
        else
            printf("Enter Ci-energy of fighter %d: ", i);

        fgets(buffer, BUFFSIZE, stdin);
        recvMsgSize = strlen(buffer);

        if (sendto(sock, buffer, recvMsgSize, 0, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) != recvMsgSize) {
            DieWithError("Mismatch in number of sent bytes");
        }
    }

    close(sock);
    return 0;
}
