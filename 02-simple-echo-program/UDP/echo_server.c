#include "headers.h"


int main(int argc, char **argv){
    int sockfd, port;
    struct sockaddr_in servaddr, cliaddr; 

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Creates a UDP Socket
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    port = atoi(argv[1]);
    servaddr.sin_port = htons(port);

    bind(sockfd, (SA*) &servaddr, sizeof(servaddr));
    printf("Server is waiting connection at port %d\t\n", port);
    handle_client(sockfd, (SA*) &cliaddr, sizeof(cliaddr));

}
