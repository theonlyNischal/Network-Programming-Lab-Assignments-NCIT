//
//  main.c
//  ConcurrentServer
//
//  Created by Chandra Mohan Jayaswal on 6/11/15.
//  Copyright (c) 2015 Chandra Mohan Jayaswal. All rights reserved.
//

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

void doprocessing (int clientFd){
    time_t ticks = time(NULL);
    char sendBuffer [1023];

    snprintf(sendBuffer, sizeof(sendBuffer), "%.24s\r\n", ctime(&ticks));
    
    /*Writing time to connected clint's socket*/
    write(clientFd, sendBuffer, sizeof(sendBuffer));
}

int main(int argc, const char * argv[]) {
    /*Varialble declaration*/
    int serverFd = 0, clientFd=0;
    struct sockaddr_in serverAddress; //For iPV4
    char sendBuffer [1023];
    pid_t pid;
    
    /*Creating Socket*/
    serverFd= socket(AF_INET,SOCK_STREAM , 0);
    
    /*Reseting sendBuffer*/
//    memset(sendBuffer,'0',sizeof(sendBuffer));
    bzero(sendBuffer,sizeof(sendBuffer));
    
    
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
        
        /*Create child process*/
        pid = fork();
        if (pid < 0) {
            perror("ERROR ON fork");
            exit(0);
        }
        
        if (pid == 0) {
            /*This is the client process*/
            close(serverFd);
            doprocessing(clientFd);
            exit(0);
        } else {
            close(clientFd);
        }
    }
    
}
