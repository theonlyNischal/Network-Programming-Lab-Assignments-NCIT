//
//  main.c
//  RWServerChat
//
//  Created by Chandra Mohan Jayaswal on 6/11/15.
//  Copyright (c) 2015 Chandra Mohan Jayaswal. All rights reserved.
//

#include <stdio.h>

#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char*argv[]){
    /*Varialble declaration*/
    int serverFd = 0, clientFd=0;
    ssize_t n;
    struct sockaddr_in serverAddress; //For iPV4
    char buffer [1023];
    
    /*Creating Socket*/
    serverFd= socket(AF_INET,SOCK_STREAM , 0);
    
    /*Reseting sendBuffer*/
    //    memset(sendBuffer,'0',sizeof(sendBuffer));
    bzero(buffer,sizeof(buffer));
    
    
    /*Setting Address Variable*/
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
    serverAddress.sin_port= htons(5000);
    
    /*Binding socket to port address*/
    bind(serverFd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
    
    /*Listeing socket*/
    listen(serverFd, 10);
    
    while (1) {
        /*Accepting connection from client*/
        clientFd = accept(serverFd,(struct sockaddr *)NULL, NULL);
        if (clientFd < 0) {
            perror("ERROR on accept");
        }
        
        /* If connection is established then start communicating */
        /*Read Data from client*/
        n = read(clientFd, buffer, sizeof(buffer));
        if (n < 0) {
            perror("ERROR reading from socket");
            return 1;
        }
        printf("Client: %s\n", buffer);
        
        /*Write a response to the client*/
        /*Ask for message from the user, this message will be read by server*/
        printf("Server:");
        bzero(buffer, sizeof(buffer)); /*Reseting receiveBuffer */
        fgets(buffer, 255, stdin);
        
        /* Send message to the server */
        n = write(clientFd, buffer, sizeof(buffer));
        if (n < 0) {
            perror("ERROR writing to socket");
            return 1;
        }
        /*Closing client socket*/
//        close(clientFd);
    }
    return 0;
}

