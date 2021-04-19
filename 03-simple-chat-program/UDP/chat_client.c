#include "headers.h"

int main (int argc, char **argv){

    int sockfd, port;
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    port = atoi(argv[2]); // ascii to integer conversion 
    servaddr.sin_port = htons(port);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Creates a UDP Socket
    chat(sockfd, (SA*) &servaddr, sizeof(servaddr));
    
}