// include necessary header files
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

// define global constants
#define MAXLINE 1024
#define LISTENQ 10 // maximum number of clients

typedef struct sockaddr SA;

int main(int argc, char **argv){
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    char buff [MAXLINE];
    time_t ticks;
    int port;
    socklen_t len;

    listenfd = socket(AF_INET, SOCK_STREAM, 0); // Creates a TCP Socket

    if(argc != 2){
    /*Checks if we are using ./daytime_tcp_server  8080 or not.
    */
    printf("Usage: ./daytime_tcp_server <port no.>\n");
    exit(1);
    }

    port = atoi(argv[1]);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

     bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
     printf("Server is waiting connection at port %d\t\n", port);
     listen(listenfd, LISTENQ);

    while(1){
        len = sizeof(cliaddr);
        connfd = accept(listenfd, (SA *) &cliaddr, &len);
        printf("Connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));
        close(connfd);
    }


}
