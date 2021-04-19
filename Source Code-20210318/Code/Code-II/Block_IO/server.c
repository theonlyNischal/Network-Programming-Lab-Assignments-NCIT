//Blocking IO Server
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
#include<arpa/inet.h>
int quit = 0;
void myhandler(int code, siginfo_t * info, void * uap){
	printf("Quiting...\n");
	quit = 1;
}
int main() {
	int sockfd,connfd;
	struct sockaddr_in local;
	size_t total = 0;
	size_t current;
	char in[1000];

	struct sigaction mysigaction;
	memset(&mysigaction, 0 , sizeof(struct sigaction));
	mysigaction.sa_flags = SA_SIGINFO;
	mysigaction.__sigaction_u.__sa_sigaction = &myhandler;
	sigaction(SIGINT, &mysigaction, NULL);
	
	memset(&local,0,sizeof(struct sockaddr_in));
	local.sin_port = htons(8080);
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket ");
		exit(0);
	}
	if(bind(sockfd, (struct sockaddr *)&local, sizeof(struct sockaddr_in)) < 0) {
		perror("bind ");
		exit(0);
	}

	if(listen(sockfd, 5) < 0) {
		perror("listen ");
		exit(0);	
	}
	if((connfd = accept(sockfd, NULL, NULL)) < 0) {
		perror("accept ");
		exit(0);
	}
	getchar();
	for(;;) {
		current=read(connfd,in,1000);
		total += current;
		printf("%zu\n",total);
		if(quit || current == 0)
			break;
	}
	close(connfd);
	close(sockfd);
	return 0;
}