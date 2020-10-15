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

int main(int argc, char **argv){
    int listenfd, connfd;
    char rbuffer[MAXLINE];
    ssize_t sn, n;
    struct sockaddr_in servaddr, cliaddr;
    char buff [MAXLINE];
    int port;
    socklen_t len;

    listenfd = socket(AF_INET, SOCK_STREAM, 0); // Creates a TCP Socket

    port = atoi(argv[1]);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

     bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
     printf("Server is waiting connection at port %d\t\n", port);
     listen(listenfd, LISTENQ);

    len = sizeof(cliaddr);
    connfd = accept(listenfd, (SA *) &cliaddr, &len);
    if(connfd >0){
    printf("Connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
    }else{
        printf("Accept Error.");
        exit(2);
    }

    while(1){
        bzero(rbuffer, sizeof(rbuffer));
        n = read(connfd, rbuffer, sizeof(rbuffer));

        if (n<0){
            perror("Error reading from socket.");
            // return 1;
            exit(2);
        }

        printf("Client: ");
        puts(rbuffer);
        printf("\n");



        sn = 0;
        sn = write(connfd, rbuffer, sizeof(rbuffer));
        if (sn<0){
            perror("Error writing to socket.");
            // return 1;
            exit(3);
        }
    }

    close(connfd);

}
