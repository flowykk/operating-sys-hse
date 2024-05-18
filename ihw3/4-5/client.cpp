#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

using namespace std;

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

    if ((argc < 3) || (argc > 4)) {    /* Test for correct number of arguments */
        fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
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

    int n = 6;
    for (int i = 0; i < n; i++) {
        printf("Enter Ci-energy of fighter %d: ", i + 1);
        fgets(buffer, BUFFSIZE, stdin);
        recvMsgSize = strlen(buffer);

        if (send(sock, buffer, recvMsgSize, 0) != recvMsgSize) {
            DieWithError("Mismatch in number of sent bytes");
        }
    }

    close(sock);
    return 0;
}
