#include "headers.h"

int main(int argc, char **argv){
    int sockfd, port;
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    port = atoi(argv[2]); // ascii to integer conversion 
    servaddr.sin_port = htons(port);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Creates a UDP Socket
    if (sockfd < 0){
        printf("Problem creating UDP socket.\n");
    }else{
        printf("Socket created successfully.\n");
    }
    send_echo(sockfd, (SA*) &servaddr, sizeof(servaddr));
    
}

// int main (int argc, char **argv){
//     int sockfd, n;
//     char recvline [MAXLINE + 1];
//     struct sockaddr_in servaddr;
//     int port;

//     ssize_t cn=0;
//     char sbuffer[1024];
//     char rbuffer[1024];
//     int value;

//     bzero(&servaddr, sizeof(servaddr));


//     if(argc != 3){
//         printf("Usage: a.out <IPaddress> <port no.>");
//     }
//     if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){ // Creates a TCP Socket
//             printf("Socket Error");
//     }

//     port = atoi(argv[2]); // ascii to integer conversion 

//     bzero(&servaddr, sizeof(servaddr)); // fills servaddr with zeros.

//     servaddr.sin_family = AF_INET;
//     servaddr.sin_port = htons(port);

//     if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){ // pton = presentatin to network, convert command line argument like 204..... to correct.
//         printf("inet_pton error for %s", argv[1]);
//     }

//     // if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0){ // Establishes a TCP Connnection
//     //     printf("Connect Error.\n");
//     //     exit(1);
//     // }

//     while(1){
//         bzero(sbuffer, sizeof(sbuffer));
//         // printf("Client_%d: ", getpid());
//         printf("Client: ");
//         fgets(sbuffer, 255, stdin);

//         // n = write(sockfd, sbuffer, sizeof(sbuffer));
//         n = sendto(sockfd, sbuffer, strlen(sbuffer),0, NULL, NULL);
//         if (n<0){
//             perror("Error.");
//         }
//         bzero(rbuffer, sizeof(rbuffer));
//         cn = recvfrom(sockfd, rbuffer, MAXLINE, 0, NULL, NULL);



//         // read

        
//         // cn = read(sockfd, rbuffer, sizeof(rbuffer));

//         if (cn<0){
//             perror("Error reading.");
//         }

//         printf("Server: ");
//         puts(rbuffer);
//         // printf("\n");
//     } // while

    
// }