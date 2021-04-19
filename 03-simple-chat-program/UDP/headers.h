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

void handle_client(int sockfd, SA *cliaddr, socklen_t cli_len){

    char rbuffer[MAXLINE];
    char sbuffer[MAXLINE];
    int n;

    for (;;){
        bzero(rbuffer, sizeof(rbuffer));
        n = recvfrom(sockfd, rbuffer, MAXLINE, 0, cliaddr, &cli_len);

        printf("Client: ");
        puts(rbuffer);
        printf("\n");

        printf("Server: ");
        bzero(sbuffer, sizeof(sbuffer));
        fgets(sbuffer, 255, stdin);
        sendto(sockfd, sbuffer, n, 0, cliaddr, cli_len);
        
    }
}


void chat(int sockfd, SA *servaddr, socklen_t serv_len){
    char sbuffer[MAXLINE], rbuffer[MAXLINE];
    int n, cn;
    while(1){
        bzero(sbuffer, sizeof(sbuffer));
        printf("Client: ");
        fgets(sbuffer, 255, stdin);

        n = sendto(sockfd, sbuffer, strlen(sbuffer),0, servaddr, serv_len);

        bzero(rbuffer, sizeof(rbuffer));
        cn = recvfrom(sockfd, rbuffer, MAXLINE, 0, NULL, NULL);
        printf("Server: ");
        puts(rbuffer);
        printf("\n");
    } // while

}

