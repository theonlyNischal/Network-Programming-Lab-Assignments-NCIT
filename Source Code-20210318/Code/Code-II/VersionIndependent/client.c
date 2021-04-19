s#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>
#include<netdb.h>

int main(int argc,char **argv) {

	struct addrinfo hints,*results,*ptr;
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;

	getaddrinfo(argv[1], "http", &hints, &results);

	for(ptr = results;ptr != NULL;ptr = ptr->ai_next) {
		if(ptr->ai_addr->sa_family == AF_INET) {
			struct sockaddr_in * addr = (struct sockaddr_in *)ptr->ai_addr;
			char ip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET,&(addr->sin_addr.s_addr),ip,INET_ADDRSTRLEN);
			printf("%s\n",ip);
		}
		else if(ptr->ai_addr->sa_family == AF_INET6) {
			struct sockaddr_in6 * addr6 = (struct sockaddr_in6 *)ptr->ai_addr;
			char ip6[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6,&(addr6->sin6_addr.s6_addr),ip6,INET6_ADDRSTRLEN);
			printf("%s\n",ip6);
		}
	}

	int sockfd = -1;

	for(ptr = results;ptr != NULL;ptr = ptr->ai_next) {
		sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if(sockfd < 0)
			continue;
		if(connect(sockfd,ptr->ai_addr,ptr->ai_addrlen) < 0) {
			close(sockfd);
			sockfd = -1;
			continue;
		}
		break;

	}
	freeaddrinfo(results);
	if (sockfd < 0) {
		return 0;
	}

	char in[1000];
	char request[1000];
	memset(request,'\0',1000);
	sprintf(request,"GET /index.html HTTP/1.1\nHost:%s\n\n",argv[1]);
	write(sockfd,request,strlen(request));
	read(sockfd,in,1000);
	puts(in);

	close(sockfd);
	return 0;
}