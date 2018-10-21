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
#define HOSTNAME "christian-Aspire-E5-575"

int main(int argc, char *argv[])
{
        struct addrinfo hints, *result, *rp;
        int sockfd;
        char buf[BUFSIZE];
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo(HOSTNAME, PORT, &hints, &result) != 0) {
                perror("addrinfo");
                exit(EXIT_FAILURE);
        }
        
        for (rp = result; rp != NULL; rp = rp->ai_next) {
                if ((sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1) {
                        perror("socket");
                        continue;
                }

                if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) {
                        break;
                }
                
                close(sockfd);
        }

        if (rp == NULL) {
                perror("NO NODES");
                exit(EXIT_FAILURE);
        }

        freeaddrinfo(result);

        while(1) {      
                printf("Type message...\n");
                fgets(buf, BUFSIZE, stdin);
                if (send(sockfd, buf, strlen(buf), 0) == -1) {
                        perror("send");
                        exit(EXIT_FAILURE);
                }
                
                if (recv(sockfd, buf, strlen(buf), 0) == -1) {
                        perror("Client recv");
                        exit(EXIT_FAILURE);
                }
        
                printf("Message sent by server: %s\n", buf);
        }
        close(sockfd);
        exit(0);
        return 0;
}  
