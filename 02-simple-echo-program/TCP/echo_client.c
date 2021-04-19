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

#define MAXLINE 1024
#define LISTENQ 10

typedef struct sockaddr SA;

int main (int argc, char **argv){
    int sockfd, n;
    char recvline [MAXLINE + 1];
    struct sockaddr_in servaddr;
    int port;

    ssize_t cn=0;
    char sbuffer[1024];
    char rbuffer[1024];
    int value;

    bzero(&servaddr, sizeof(servaddr));


    if(argc != 3){
        printf("Usage: a.out <IPaddress> <port no.>");
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ // Creates a TCP Socket
            printf("Socket Error");
    }

    port = atoi(argv[2]); // ascii to integer conversion 

    bzero(&servaddr, sizeof(servaddr)); // fills servaddr with zeros.

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){ // pton = presentatin to network, convert command line argument like 204..... to correct.
        printf("inet_pton error for %s", argv[1]);
    }

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0){ // Establishes a TCP Connnection
        printf("Connect Error.\n");
        exit(1);
    }

    while(1){
        bzero(sbuffer, sizeof(sbuffer));
        // printf("Client_%d: ", getpid());
        printf("Client: ");
        fgets(sbuffer, 255, stdin);

        n = write(sockfd, sbuffer, sizeof(sbuffer));

        if (n<0){
            perror("Error.");
        }

        // read

        bzero(rbuffer, sizeof(rbuffer));
        cn = read(sockfd, rbuffer, sizeof(rbuffer));

        if (cn<0){
            perror("Error reading.");
        }

        printf("Server: ");
        puts(rbuffer);
        // printf("\n");
    } // while

    
}