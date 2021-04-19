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
    fd_set reads, active_sockets;
    int listenfd, n, s;
    char recvline [MAXLINE + 1];
    struct sockaddr_in servaddr;
    int port, l1, l2;
    char temp_ques[MAXLINE], temp_ans[MAXLINE];

    ssize_t cn=0;
    char sbuffer[MAXLINE], rbuffer[MAXLINE], username[10];
    int value;
    bzero(&servaddr, sizeof(servaddr));

    // Specifying timeout
    struct timeval timeout;
	timeout.tv_sec = 100;
	timeout.tv_usec = 0;


    // Check for correct format: ./quiz_server -p <port>
    if (argc!= 7){
        perror("Usage: ./quiz_client -h <server_address> -p <port> -u <username>\n");
        exit(0);
    }
    if(strlen(argv[6])>10){
        perror("Please use username less than 10 characters long.\n");
        exit(1);
    }
    bzero(username, MAXLINE);
    strcpy(username, argv[6]);
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ // Creates a TCP Socket
            printf("Cannot Create Socket.");
    }

    port = atoi(argv[4]); // ascii to integer conversion 

    bzero(&servaddr, sizeof(servaddr)); // fills servaddr with zeros.

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[2], &servaddr.sin_addr) <= 0){ // pton = presentatin to network, convert command line argument like 204..... to correct.
        printf("inet_pton error for %s", argv[2]);
    }

    // connect outbound to a server
    if (connect(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0){ // Establishes a TCP Connnection
        printf("Connect Error.\n");
        exit(2);
    }else{
        // n = write(listenfd, username, strlen(username));
        n = send(listenfd,username,strlen(username),0);	
        if(n<0){
            perror("Cannot send username.\n");
            exit(0);
        }


    }//connect

    // active_sockets stores all of the active sockets.
    // right now only listenfd is active.
    FD_ZERO(&active_sockets);
    FD_SET(listenfd, &active_sockets);
	FD_SET(0,&active_sockets);


    while(1){
        reads = active_sockets;
        s = select(FD_SETSIZE, &reads, 0, 0, &timeout);
        if(s<0){
            printf("Error in Select Function.\n");
            exit(0);
        }

        if(FD_ISSET(0,&reads)){ // Input from keyboard.
            bzero(rbuffer, MAXLINE);
            fgets(rbuffer, MAXLINE, stdin);

            if(strncmp(rbuffer, "2", 1)==0){
                bzero(temp_ans, MAXLINE);
                bzero(temp_ques, MAXLINE);
                sscanf(rbuffer, "2%[^?]?(%[^)])", temp_ques, temp_ans);
                l1 = strlen(temp_ques);
                l2 = strlen(temp_ans);

                if(l1==0 || l2==0){
                    printf("\nWrong question format.\n");
                    printf("Use <2Question?(Answer)> to enter new question.\n");
                }else{

                    // n = write(listenfd, rbuffer, strlen(rbuffer));
                    n = send(listenfd,rbuffer,strlen(rbuffer),0);	
                    if (n<0){
                        perror("Error.");
                    }
                }

            }else if(strncmp(rbuffer, "4", 1)==0){
                // n = write(listenfd, rbuffer, strlen(rbuffer));
                n = send(listenfd,rbuffer,strlen(rbuffer),0);	
                if (n<0){
                    perror("Error.");
                }
            }else if(strncmp(rbuffer, "QUIT", 4)==0){
                printf("Quitting....\n");
                exit(1);

            }else{
                printf("Wrong input format.\n");
                printf("Use <2Question?(Answer)> to add new quiz.\n");
                printf("use <4Answer> to answer active question.\n");
                printf("use <QUIT> to quit.\n");
            }					
                        
        }else if(FD_ISSET(listenfd, &reads)){ //Receive something from socket
            bzero(rbuffer, MAXLINE);
            // n = read(listenfd, rbuffer, sizeof(rbuffer));
            n=recv(listenfd,rbuffer,MAXLINE,0);
            rbuffer[n]=0;

            if (n<=0){
                perror("Error reading.");
                exit(0);
            }
            printf("%s\n", rbuffer);
        }
        
    } // while  
}




