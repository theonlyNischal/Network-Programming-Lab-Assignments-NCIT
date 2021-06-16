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
#define MAXCLIENT 10
#define NAMELENGTH 10

typedef struct sockaddr SA;

//Global Var
int client_count = 0;
int quiz_count = 0;
char active_ques[MAXLINE], active_ans[MAXLINE];
int active_socket;
int quiz_active=0;
char global_temp_msg[MAXLINE];

//Quiz
typedef struct {
	int client_fd;
	char question[100]; 
	char answer[100];

}QUIZ;
QUIZ quiz_list[100];

void add_quiz(int client_fd, char* question, char* answer){
    quiz_list[quiz_count].client_fd = client_fd;
    strcpy(quiz_list[quiz_count].question, question);
    strcpy(quiz_list[quiz_count].answer, answer);
    quiz_count = quiz_count+1;
    printf("New quiz added.\n");
    printf("Question: %s\nAnswer: %s\n", question, answer);
}

void remove_quiz(int quiz_position){
    int i=0;
    if(quiz_count==1 || quiz_position==quiz_count-1){
        bzero(quiz_list[quiz_position].question,100); 
		sprintf(quiz_list[quiz_position].question,"");
		bzero(quiz_list[quiz_position].answer,100); 
		sprintf(quiz_list[quiz_position].answer,"");
    }else{
        for(i=quiz_position; i<quiz_count-1; i++){
            bzero(quiz_list[i].question,100);			
			bzero(quiz_list[i].answer,100); 
			quiz_list[i].client_fd=quiz_list[i+1].client_fd;
			strcpy(quiz_list[i].question, quiz_list[i+1].question);
			strcpy(quiz_list[i].answer, quiz_list[i+1].answer);

        }
        bzero(quiz_list[quiz_count-1].question,100); 
		sprintf(quiz_list[quiz_count-1].question,"");
		bzero(quiz_list[quiz_count-1].answer,100); 
		sprintf(quiz_list[quiz_count-1].answer,"");
    }
    quiz_count = quiz_count - 1;
}

//Client
typedef struct 
{
	char username[NAMELENGTH]; 
	int client_fd;
    // int score;
  
}CLIENTS;
CLIENTS users[10];

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

void place_new_question(){
    bzero(active_ques, MAXLINE);
    bzero(active_ans, MAXLINE);
    if(quiz_count==0){
        printf("No Question to place...\n");
        sprintf(active_ques, "1Enter new question\n");
        sprintf(active_ans, "");
        active_socket = 0;
        quiz_active=0;
        send_msg_to_all(active_ques);
        
    }else if(quiz_count==1){
        sprintf(active_ques, "3%s", quiz_list[0].question);
        strcpy(active_ans, quiz_list[0].answer);
        active_socket = quiz_list[0].client_fd;
        remove_quiz(0);
        quiz_active=1;
        send_msg_to_all_except_one(active_ques, active_socket);
        bzero(global_temp_msg, MAXLINE);
        sprintf(global_temp_msg, "Your question '%s' is active.\n", active_ques);
        send(active_socket, global_temp_msg, strlen(global_temp_msg),0);	
        

    }else{
        int rand_num=0;
        rand_num = 1+rand()%(quiz_count);
        sprintf(active_ques, "3%s", quiz_list[rand_num-1].question);
        strcpy(active_ans, quiz_list[rand_num-1].answer);
        active_socket = quiz_list[rand_num-1].client_fd;
        remove_quiz(rand_num-1);
        quiz_active=1;
        send_msg_to_all_except_one(active_ques, active_socket);
        bzero(global_temp_msg, MAXLINE);
        sprintf(global_temp_msg, "Your question '%s' is active.\n", active_ques);
        send(active_socket, global_temp_msg, strlen(global_temp_msg),0);	

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

    place_new_question();
    while(1){
        reads = active_sockets;
        s = select(FD_SETSIZE, &reads, 0, 0, &timeout);
        if(s<0){
            printf("Error in Select Function.\n");
            exit(0);
        }else if(s==0){
            sprintf(temp_message, "Time exceeded.\nCorrect answer:%s", active_ans);
            send_msg_to_all(temp_message);

            place_new_question();

            
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
                sprintf(rbuffer, "%s joined the quiz.\n", rbuffer);
                send_msg_to_all_except_one(rbuffer, connfd);
                bzero(rbuffer, MAXLINE);
                sprintf(rbuffer, "0Welcome to Quiz!!\n%s", active_ques);
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