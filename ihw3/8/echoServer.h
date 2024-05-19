#include <cstdio>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <cstdlib>     /* for atoi() and exit() */
#include <cstring>     /* for memset() */
#include <unistd.h>     /* for close() */

void DieWithError(const char *mess);  /* Error handling function */
int AcceptTCPClient(int servSock);
