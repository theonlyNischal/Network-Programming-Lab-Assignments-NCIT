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
#define NAMELENGTH 10

typedef struct sockaddr SA;

//Global Var
int client_count = 0;
int quiz_count = 0;
char active_ques[MAXLINE], active_ans[MAXLINE];
int active_socket;
int quiz_active=0;
char global_temp_msg[MAXLINE];

//Client
typedef struct 
{
	char username[NAMELENGTH]; 
	int client_fd;
    // int score;
  
}CLIENTS;
CLIENTS users[LISTENQ];

void add_client(int fd, char* username)
{
	users[client_count].client_fd = fd;
	strcpy(users[client_count].username, username);	
	client_count = client_count + 1;
	printf("0Welcome to the Quiz %s.\n",username);
}

void remove_client(int clientposition)
{
	int i=0;
	if(client_count==1 || clientposition==client_count-1)
	{
		bzero(users[clientposition].username,10); 
		sprintf(users[clientposition].username,"");
		
	}else{
		for(i= clientposition; i < client_count-1; i++)
		{
			bzero(users[i].username,100);			
			
			users[i].client_fd=users[i+1].client_fd;
			strcpy(users[i].username, users[i+1].username);
			
			
		}
		bzero(users[client_count-1].username,10); 
		sprintf(users[client_count-1].username,"");
		
	}
		
	
	client_count = client_count - 1;
	
}

void send_msg_to_client(char* message, char* username, int sender_fd){
    int client_found = 0;
    char temp[MAXLINE];
    printf("Sending message to %s.\n", username);
    int i = 0;
    for (i=0; i<=client_count; i++){
        if(strcmp(users[i].username, username) == 0){
            client_found = 1;
            
            send(users[i].client_fd, message, strlen(message), 0);
        }
    }
    if (client_found == 0){
        // client not found.
        bzero(message, MAXLINE); 
		sprintf(message, "%s not active.", username);
        send(sender_fd, message, strlen(message), 0);

        
    }
}

void send_msg_to_all(char* message){
    printf("Sending message to all: %s", message);
    int i=0;
    for(i=0; i<=client_count; i++){
        // write(users[i].client_fd, message, strlen(message));
        send(users[i].client_fd,message,strlen(message),0);	
    }
}

void send_msg_to_all_except_one(char* message, int client_fd){
    int i=0;
    for(i=0; i<=client_count; i++){
        if(client_fd!=users[i].client_fd){
            // write(users[i].client_fd, message, strlen(message));
            send(users[i].client_fd,message,strlen(message),0);	
        }
    }
}


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


    // Specifying timeout
    struct timeval timeout;
	timeout.tv_sec = 200;
	timeout.tv_usec = 0;

    // Check for correct format: ./quiz_server -p <port>
    if (argc!= 3){
        perror("Usage: ./quiz_server -p <port>");
        exit(0);
    }

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
        s = select(FD_SETSIZE, &reads, 0, 0, &timeout);
        if(s<0){
            printf("Error in Select Function.\n");
            exit(0);
        }else if(s==0){
            sprintf(temp_message, "Time exceeded.\n");

            
            timeout.tv_sec = 200;
	        timeout.tv_usec = 0;


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

                    if (n<0){
                        perror("Cannot read username.\n");
                        return 1;
                    }
                            
                }else{
                    printf("Accept Error.\n");
                }
                // Add new connection to active connection
                FD_SET(connfd, &active_sockets);
                add_client(connfd, rbuffer);
                sprintf(rbuffer, "%s joined the chat.\n", rbuffer);
                send_msg_to_all_except_one(rbuffer, connfd);
                bzero(rbuffer, MAXLINE);
                sprintf(rbuffer, "0Welcome to Chat!!\n%s", active_ques);
                send(connfd, rbuffer, strlen(rbuffer),0);
                

            } else{ // Not New Connection
                int client_counter = 0;

                while(client_counter<client_count){

                    if(FD_ISSET(users[client_counter].client_fd, &reads)){
                        // Received something from client.
                        bzero(rbuffer, MAXLINE);
                        // n = read(users[client_counter].client_fd, rbuffer, sizeof(rbuffer));
                        n=recv(users[client_counter].client_fd,rbuffer,MAXLINE,0);	
                        if(n<=0){
                            printf("%s left the Chat.\n", users[client_counter].username);
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
                                send_msg_to_client(temp_ques, temp_ans, users[client_counter].client_fd);




                            }else if(strncmp(rbuffer, "4", 1)==0){
                                sscanf(rbuffer, "4%s", temp_ans);
                                puts(rbuffer);
                                puts(active_ans);
                                

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