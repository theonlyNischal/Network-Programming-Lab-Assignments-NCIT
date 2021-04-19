#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<signal.h>

int quit = 0;

void myhandler(int code, siginfo_t * info, void * uap){
	printf("Quiting...\n");
	quit = 1;
}

int main() {

	int sockfd;
	struct sockaddr_in server;
	char out[1000];
	size_t total = 0;

	struct sigaction mysigaction;
	memset(&mysigaction, 0 , sizeof(struct sigaction));
	mysigaction.sa_flags = SA_SIGINFO;
	mysigaction.__sigaction_u.__sa_sigaction = &myhandler;
	sigaction(SIGINT, &mysigaction, NULL);

	memset(&server,0,sizeof(struct sockaddr_in));
	server.sin_port = htons(8080);
	server.sin_family = AF_INET;
	inet_pton(AF_INET,"127.0.0.1",&server.sin_addr.s_addr);
	//inet_pton(AF_INET,"192.168.100.251",&server.sin_addr.s_addr);
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("socket ");
		exit(0);
	}



	if(connect(sockfd,(struct sockaddr *)&server,sizeof(struct sockaddr_in)) < 0) {
		perror("bind");
		exit(0);
	}

	int size = 200000;
	socklen_t len = sizeof(int);

	
	struct linger l;
	l.l_onoff = 1;
	l.l_linger = 0;


	if(setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &l, sizeof(struct linger)) < 0)
		printf("Couldn't set data.\n");
	
	
	
	if(getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, &len) < 0)
		printf("Couldn't obtain data.\n");
	else
		printf("SNDBUF: %d\n",size);

	for (; ;) {
		total += write(sockfd,out,1000);
		printf("%zu\n",total);
		if(quit)
			break;
	}

	close(sockfd);
	return 0;
}