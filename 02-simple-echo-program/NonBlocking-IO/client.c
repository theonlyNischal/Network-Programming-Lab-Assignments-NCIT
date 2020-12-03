#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <netdb.h>

#define MAXLINE 1024
#define LISTENQ 10
typedef struct sockaddr SA;

int main(int argc, char **argv){
    int sockfd, port, s, len, cn, n;
    struct sockaddr_in servaddr, clientaddr;
    char rbuffer[MAXLINE], wbuffer[MAXLINE];
    fd_set master, read_fds;
    port = atoi(argv[2]); // ascii to integer conversion 
    printf("==============================\n");
    printf("Client Started.\n");
    printf("Port: %d\n", port);
    printf("==============================\n");

    // Register server address.
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Creates a UDP Socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sockfd < 0){
        printf("Problem creating UDP socket.\n");
        exit(EXIT_FAILURE);
    }else{
        printf("Socket created successfully.\n\n\n\n");
    }

    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientaddr.sin_port = htons(port);

    // initialize current set
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);
    FD_SET(0, &read_fds);

    while(1){
        // Makes backup as select is destructive.
        master = read_fds;
        // Network or Keyboard.
        s = select(FD_SETSIZE, &master, NULL, NULL, NULL);

        if (s<0){
            perror("Select Error.\n");
            exit(EXIT_FAILURE);
        }

        // File Descriptior: Standard Input
        if (FD_ISSET(0, &master)){
            bzero(wbuffer, sizeof(wbuffer));
            fgets(wbuffer, 255, stdin);
            cn = sendto(sockfd, wbuffer, strlen(wbuffer), 0, (SA *) &servaddr, sizeof(servaddr));

            if(cn<0){
                printf("Cannot send data.\n");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
        } // standard input

        if(FD_ISSET(sockfd, &master)){
            bzero(rbuffer, sizeof(rbuffer));
            n = recvfrom(sockfd, rbuffer, MAXLINE, 0, (SA*) &clientaddr, &len);

            if(n<0){
                printf("Receive Error.\n");
            }
            printf("==============================\n");
            printf("Echoed from Server.\n");
            printf("Server: ");
            puts(rbuffer);
            
        }

    }

    
}