#include <stdio.h>
#include <sys/socket.h> // basic socket defination
#include <netinet/in.h> // sockaddr_in{} and other Internet definations
#include <arpa/inet.h> // inet(3) functionis
#include <stdlib.h> 
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h> // basic system data types
#include <time.h>

// define global constants
#define MAXLINE 1024
#define LISTENQ 10 // maximum number of clients

typedef struct sockaddr SA;


int main (int argc, char **argv){
    int sockfd, n;
    char recvline [MAXLINE + 1];
    struct sockaddr_in servaddr;
    int port;

    if(argc != 3){
        /*Checks if we are using ./daytime_tcp_client 127.0.0.1 8080 or not.
        */
        printf("Usage: ./daytime_tcp_client <IP Address> <port no.>\n");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ // Creates a TCP Socket
            printf("Socket Error.\n");
            exit(1);
    }

    port = atoi(argv[2]); // ascii to integer conversion 

    bzero(&servaddr, sizeof(servaddr)); // fills servaddr with zeros.

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){ 
        /* pton = presentatin to network, 
        This function convert the IP Adress in argv[1] like 127.0.0.1 from 
        character string to binary form, then it copies the binary form to
        @servaddr.sin_addr.

        */
        printf("inet_pton error for %s.\n", argv[1]);
        exit(1);
    }

    // connect() tries to establish a TCP connection.
    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0){ 
        printf("Connect Error.\n");
        exit(1);
    }

    while ((n=read(sockfd, recvline, MAXLINE)) > 0){ // reads server reply.
        recvline[n] =0;

        if (fputs(recvline, stdout) == EOF){
            printf("fputs error.\n");
        }
    }

    if (n<0){
        printf("Read error.\n");
        exit(1);
    }  
}