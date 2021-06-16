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

#define LISTENQ 10
#define MAXLINE 1024

typedef struct sockaddr SA;


int main(int argc, char **argv){
    fd_set reads, active_sockets;
    int listenfd, connfd, max_socket;
    int port;
    int i, j, s, n;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    char rbuffer [MAXLINE], buff[MAXLINE];
    char temp_ques[MAXLINE], temp_ans[MAXLINE];
    char temp_message[MAXLINE];

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Cannot Create Socket.");
        exit(0);

    } // Creates a TCP Socket
    
    port = atoi(argv[2]); // ascii to integer conversion 

    bzero(&servaddr, sizeof(servaddr)); // fills servaddr with zeros.
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr))<0){
        printf("Bind Error.\n");
        exit(0);
    }
    printf("Server is waiting connection at port %d\t\n", port);
    if(listen(listenfd, LISTENQ) < 0){
        printf("Listen Error.");
        exit(0);
    }

    // active_sockets stores all of the active sockets.
    // right now only listenfd is active.
    FD_ZERO(&active_sockets);
    FD_SET(listenfd, &active_sockets);
    max_socket = listenfd; // max_socket holds the largest socket descriptor.

    while(1){
        reads = active_sockets;
        s = select(FD_SETSIZE, &reads, 0, 0, NULL);
        if(s<0){
            printf("Error in Select Function.\n");
            exit(0);
        }else if(s==0){
            // handle timeout
        }else{
            //New Connection as listenfd is ready.
            if(FD_ISSET(listenfd, &reads)){
                cliaddr_len = sizeof(cliaddr);
                connfd = accept(listenfd, (SA *) &cliaddr, &cliaddr_len); // accept an inbound connection
                if(connfd>0){
                    // printf("Connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
                    bzero(rbuffer, MAXLINE);
                    // n = read(connfd, rbuffer, sizeof(rbuffer));
                    n=recv(connfd,rbuffer,MAXLINE,0);
                            
                }else{
                    printf("Accept Error.\n");
                }
                // Add new connection to active connection
                FD_SET(connfd, &active_sockets);
                

            } else{ // Not New Connection
                int client_counter = 0;

                while(client_counter<client_count){

                    if(FD_ISSET(users[client_counter].client_fd, &reads)){
                        // Received something from client.
                        bzero(rbuffer, MAXLINE);
                        // n = read(users[client_counter].client_fd, rbuffer, sizeof(rbuffer));
                        n=recv(users[client_counter].client_fd,rbuffer,MAXLINE,0);	
                        if(n<=0){
                            printf("%s left the Quiz.\n", users[client_counter].username);
                            // FD_SET(users[client_counter].client_fd,&active_sockets);//removing from select
                            FD_CLR(users[client_counter].client_fd, &active_sockets);
                            remove_client(client_counter);
                            // close(users[client_counter].client_fd);
                            client_counter = client_counter - 1;
                            

                        } else{
                            printf("Received Message.\n");
                            puts(rbuffer);
                            if(strncmp(rbuffer, "2", 1)==0){
                                sscanf(rbuffer, "2%[^(](%[^)])", temp_ques, temp_ans);
                                // Add quiz
                                add_quiz(users[client_counter].client_fd, temp_ques, temp_ans);
                                if(quiz_count==1 && quiz_active== 0){
                                    place_new_question();
                                    timeout.tv_sec = 100;
	                                timeout.tv_usec = 0;
                                }


                            }else if(strncmp(rbuffer, "4", 1)==0){
                                sscanf(rbuffer, "4%s", temp_ans);
                                puts(rbuffer);
                                puts(active_ans);
                                if(strcmp(temp_ans, active_ans)==0){
                                    printf("Correct Answer....\n");
                                    bzero(rbuffer, MAXLINE);
                                    sprintf(rbuffer, "\nQuestion: %s\nAnswer: %s\nWinner: %s\n", active_ques, active_ans, users[client_counter].username);
                                    send_msg_to_all(rbuffer);
                                    place_new_question();
                                    timeout.tv_sec = 200;
	                                timeout.tv_usec = 0;
                                }else{
                                    printf("Incorrect Answer....\n");
                                    // strcpy(temp_message)
                                    bzero(temp_message, MAXLINE);
                                    sprintf(temp_message, "Your answer: '%s' is not correct.\n", temp_ans);
                                    // write(users[i].client_fd, temp_message, strlen(temp_message));
                                    send(users[client_counter].client_fd,temp_message,strlen(temp_message),0);	
                                }

                            }

                        }
                        

                    }
                    client_counter = client_counter + 1; // increase number of checked client.
                } // while
            

            }

        } // s-else
    //     close(connfd);
    } // while



}