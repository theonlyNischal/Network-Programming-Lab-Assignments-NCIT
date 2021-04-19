//
//  main.c
//  Server
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

int main(int argc, char*argv[]){
    /*Varialble declaration*/
    int serverFd = 0, clientFd=0;
    ssize_t writeLen;
    struct sockaddr_in serverAddress; //For iPV4
    char sendBuffer [1023];
    time_t ticks;
    int value;
    
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
    value = bind(serverFd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
    printf("Value: %d", value);
    if (value < 0) {
        perror("Errog on binding");
    }
    
    /*Listeing socket*/
    listen(serverFd, 10);
    
    while (1) {
        /*Accepting connection from client*/
        clientFd = accept(serverFd,(struct sockaddr *)NULL, NULL);
        
        ticks = time(NULL);
        snprintf(sendBuffer, sizeof(sendBuffer), "%.24s\r\n", ctime(&ticks));
        
        /*Writing time to connected clint's socket*/
        writeLen = write(clientFd, sendBuffer, sizeof(sendBuffer));
        
        perror("Running Server");

        /*Closing client socket*/
        close(clientFd);
//        sleep(1);
        
    }
    
}

