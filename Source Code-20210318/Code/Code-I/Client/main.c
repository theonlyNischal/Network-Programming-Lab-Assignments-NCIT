//
//  main.c
//  Client
//
//  Created by Chandra Mohan Jayaswal on 6/11/15.
//  Copyright (c) 2015 Chandra Mohan Jayaswal. All rights reserved.
//

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
    int clientFd = 0;
    ssize_t n= 0;
    char receiveBuffer[1024];
    struct sockaddr_in serverAddress;
    int value;
    
    
//    memset(receiveBuffer, '0', sizeof(receiveBuffer)); /*Reseting receiveBuffer */
//    memset(&serverAddress, '0', sizeof(serverAddress)); /*Reseting serverAddress */

    bzero(receiveBuffer, sizeof(receiveBuffer)); /*Reseting receiveBuffer */
    bzero(&serverAddress, sizeof(serverAddress)); /*Reseting serverAddress */

    
    /*Creating Client Socket*/
    clientFd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientFd <0) {
         printf("Unable to create client socket\n");
        return 1;
    }
    
    /*Setting different parameters for serverAddress*/
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5000);
    value = inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
    if (value <= 0) {
        printf("Unable to set different parameters for serverAddress");
        return 1;
    }
    
    /*Connecting to the Server whose iP address is 127.0.0.1 and port number is 5000*/
    value = connect(clientFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (value <0) {
        printf("Unable to connect server");
        return 1;
    }
   
    /*Reading data from Server*/
    n = read(clientFd, receiveBuffer, sizeof(receiveBuffer));
    if (n < 0) {
        perror("ERROR reading from socket");
        return 1;
    }
    printf("%s\n", receiveBuffer);

    return 0;
}

