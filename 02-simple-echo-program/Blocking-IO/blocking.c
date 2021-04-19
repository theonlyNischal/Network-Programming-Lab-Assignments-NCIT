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

void get_time(char *bufttime){
    time_t tim;
    struct tm *timeinfo;
    time(&tim);
    timeinfo = localtime(&tim);
    strftime(bufttime, 21, "%Y:%m:%d, %H:%M:%S", timeinfo);
}

int main(int argc, char **argv){
    int sockfd, port;
    int n, cn, len;
    struct sockaddr_in servaddr, cliaddr; 
    char rbuffer[MAXLINE], wbuffer[MAXLINE], str[MAXLINE];
    char *tim = NULL;
    tim = malloc(sizeof(char) * 100);

    // Creates a UDP socket.
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        printf("Problem Creating Socket.\n");
        exit(EXIT_FAILURE);
    }

    // Register Server Address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    port = atoi(argv[1]);
    servaddr.sin_port = htons(port);

    // Binding address to a newly created socket
    if (bind(sockfd, (SA*) &servaddr, sizeof(servaddr)) < 0){
        printf("Problem in Binding address to the socket.");
        exit(EXIT_FAILURE);
    }
    get_time(tim);
    // Printing Server Information
    printf("==============================\n");
    printf("Server is waiting connection.\n");
    printf("Port: %d\n", port);
    printf("Time: %s\n", tim);
    printf("==============================\n");

    while(1){
        bzero(rbuffer, sizeof(rbuffer));
        printf("\n\n\n............BLOCKED............\n\n\n");
        n = recvfrom(sockfd, rbuffer, MAXLINE, 0, (SA*) &cliaddr, &len);
        if (n < 0){
            if (errno == EWOULDBLOCK){
                perror("EWOULDBLOCK error.\n");
            }else{
                perror("Recvfrom");
            }
        }else{
            get_time(tim);
            printf("==============================\n");
            printf("Recieved datagram packet.\n");
            printf("Time: %s\n", tim);
            printf("Port: %d\n", port);
            printf("From: %s\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, str, sizeof(str)));
            printf("Recieved: %s\n", rbuffer);
            printf("==============================\n");
            printf("Echoing the same message.\n");
            cn = sendto(sockfd, rbuffer, n, 0, (SA *) &cliaddr, len);
            get_time(tim);
            if (n == cn){
                printf("Sent at %s\n", tim);
                printf("==============================\n");
            }

        }
    }



}
