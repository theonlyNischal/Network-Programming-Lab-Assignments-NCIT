/* At first let's import all the necessary system packages*/
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


/* defining some constants that we will use later */

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

    // This creates a socket.
    // argument 1 = Network Family, argument 2 = Familty Type
    listenfd = socket(AF_INET, SOCK_STREAM, 0); 

    // convert from ascii to integer.
    port = atoi(argv[1]);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

     bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
     printf("Server is waiting connection at port %d\t\n", port);
     // listen at listenfd
     listen(listenfd, LISTENQ);

    for(;;){
        len = sizeof(cliaddr);
        // Accepts a connections
        connfd = accept(listenfd, (SA *) &cliaddr, &len);
        if(connfd >0){
            printf("Connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, buff, 
                sizeof(buff)), ntohs(cliaddr.sin_port));
        }else{
            printf("Accept Error.");
        }

        if (fork()==0){
            // first close the listening socket
            close(listenfd);
            while(1){
                    bzero(rbuffer, sizeof(rbuffer));
                    n = read(connfd, rbuffer, sizeof(rbuffer));

                    if (n<0){
                        perror("Error reading from socket.");
                        return 1;
                    }

                    printf("Client Says: ");
                    puts(rbuffer);
                    sn = 0;
                    sn = write(connfd, rbuffer, sizeof(rbuffer));
                    if (sn<0){
                        perror("Error writing to socket.");
                        exit(3);
                    }
                }
            close(connfd);
        }

        close(connfd);
    } // end fo the for loop.

    

    

}
