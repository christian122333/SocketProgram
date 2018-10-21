#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define BACKLOG 10
#define PORT "5666"
#define BUFSIZE 250
void handleClient(const int *clntSock);

int main(int argc, char *argv[])
{
        struct addrinfo hints, *result, *rp;
        int servSock, clntSock;
        struct sockaddr_storage peer_addr;
        socklen_t sin_size;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        if (getaddrinfo(NULL, PORT, &hints, &result) != 0) {
                perror("addrinfo");
                exit(EXIT_FAILURE);
        }

        for (rp = result; rp != NULL; rp = rp->ai_next) {
                if ((servSock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1) {
                        perror("socket");
                        continue;
                }                        
                
                if (bind(servSock, rp->ai_addr, rp->ai_addrlen) == 0) {
                        break;
                }
                
                close(servSock);
        }

        if (rp == NULL) {
                perror("NO NODES");
                exit(EXIT_FAILURE);
        }
        
        
        freeaddrinfo(result);

        if (listen(servSock, BACKLOG) == -1) {
                perror("listen");
                exit(EXIT_FAILURE);
        }

        printf("Server is listening...\n");
        for (;;) {
                sin_size = sizeof(peer_addr);
                clntSock = accept(servSock, (struct sockaddr *)&peer_addr, &sin_size);
                if (clntSock == -1) {
                        perror("Accept");
                        exit(EXIT_FAILURE);
                }
                printf("FORK \n");
                if (!fork()) {    
                        close(servSock);
                        handleClient(&clntSock);
                }
                close(clntSock);
        }

        return 0;                
}

void handleClient(const int *clntSock)
{
        char buf[BUFSIZE];
        int numbytes;
        for (;;) {
                if (numbytes = recv(*clntSock, buf, BUFSIZE, 0) == -1) {
                        perror("recv");
                        exit(EXIT_FAILURE);
                }
                printf("Number of bytes received: %d\n", numbytes);                
                if (send(*clntSock, buf, numbytes, 0) == -1) {
                        perror("send");
                        exit(EXIT_FAILURE);
                }
        }
        close(*clntSock);
        exit(0);
}

