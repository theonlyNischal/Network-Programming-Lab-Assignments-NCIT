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
    int n;
    for (;;){
        bzero(rbuffer, sizeof(rbuffer));
        n = recvfrom(sockfd, rbuffer, MAXLINE, 0, cliaddr, &cli_len);
        printf("Recieved: %s\n", rbuffer);
        sendto(sockfd, rbuffer, n, 0, cliaddr, cli_len);
        
    }
}

void send_echo(int sockfd, SA *servaddr, socklen_t serv_len){
    char rbuffer[MAXLINE], wbuffer[MAXLINE];
    int n, cn;
    while(1){
            bzero(wbuffer, sizeof(wbuffer));
            bzero(rbuffer, sizeof(rbuffer));

            printf("Client: ");
            fgets(wbuffer, 255, stdin);
            n = sendto(sockfd, wbuffer, strlen(wbuffer),0, servaddr, serv_len);

            if (n<0){
                perror("Error sending message to server");
            }
            
            cn = recvfrom(sockfd, rbuffer, MAXLINE, 0, NULL, NULL);
            printf("Server: ");
            puts(rbuffer);

    }
}