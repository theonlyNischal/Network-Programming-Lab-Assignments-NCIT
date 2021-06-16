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
    printf("I am in the main.");

    int listenfd;
    int n, port, s;
    char id_number[MAXLINE];
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    fd_set reads, active_sockets;
    char rbuffer[MAXLINE];

    // Check for correct format: ./tcpInfoClient -h <serveraddress> -p <port> -n IDNumber
    if (argc!= 7){
        perror("Usage: ./tcpInfoClient -h <serveraddress> -p <port> -n IDNumber\n");
        exit(0);
    }
    bzero(id_number, MAXLINE);
    strcpy(id_number, argv[6]);

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
            printf("Cannot create socket.");
            exit(0);
    } // Creates a TCP Socket

    port = atoi(argv[4]); // ascii to integer conversion

    bzero(&servaddr, sizeof(servaddr)); // fills servaddr with zeros.
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, argv[2], &servaddr.sin_addr) <= 0){ // pton = presentatin to network, convert command line argument like 204..... to correct format.
        printf("inet_pton error for %s", argv[2]);
    } 

    // connect outbound to a server
    if (connect(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0){ // Establishes a TCP Connnection
        printf("Connect error.\n");
        exit(2);
    }else{
        printf("........");
        // n = write(listenfd, id_number, strlen(id_number));
        n = send(listenfd,id_number,strlen(id_number),0);	
        if(n<0){
            perror("Cannot send id_number.\n");
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
        s = select(FD_SETSIZE, &reads, 0, 0, NULL);
        if(s<0){
            printf("Error in Select function.\n");
            exit(0);
        }

        if(FD_ISSET(0,&reads)){ // Input from keyboard.
            bzero(rbuffer, MAXLINE);
            fgets(rbuffer, MAXLINE, stdin);

            if(strncmp(rbuffer, "2Again", 6)==0){
                puts("Okay, Again??\n");
                n = send(listenfd, rbuffer, strlen(rbuffer),0);
                if (n<0){
                    perror("Error.");
                }
                // bzero(temp_ans, MAXLINE);
                // bzero(temp_ques, MAXLINE);
                // sscanf(rbuffer, "2%[^?]?(%[^)])", temp_ques, temp_ans);
                // l1 = strlen(temp_ques);
                // l2 = strlen(temp_ans);

                // if(l1==0 || l2==0){
                //     printf("\nWrong question format.\n");
                //     printf("Use <2Question?(Answer)> to enter new question.\n");
                // }else{

                //     // n = write(listenfd, rbuffer, strlen(rbuffer));
                //     n = send(listenfd,rbuffer,strlen(rbuffer),0);	
                //     if (n<0){
                //         perror("Error.");
                //     }
                // }

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