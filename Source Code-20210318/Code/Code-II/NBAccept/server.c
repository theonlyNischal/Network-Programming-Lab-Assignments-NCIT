#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/select.h>

int run = 1;

void myhandler(int code, siginfo_t * info, void * uap){
	run = 0;
}

int main() {

	struct sigaction mysigaction;
	memset(&mysigaction, 0 , sizeof(struct sigaction));
	mysigaction.sa_flags = SA_SIGINFO;
	mysigaction.__sigaction_u.__sa_sigaction = &myhandler;
	sigaction(SIGINT, &mysigaction, NULL);

	int sockfd;
	int connfd = -1;
	struct sockaddr_in local;
	memset(&local, 0, sizeof(struct sockaddr_in));
	local.sin_family = AF_INET;
	local.sin_port = htons(8080);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket: ");
		return 0;
	}

	if(bind(sockfd, (struct sockaddr *)&local, sizeof(struct sockaddr_in)) < 0) {
		perror("bind: ");
		close(sockfd);
		return 0;
	}
	if(listen(sockfd, 5) < 0) {
		perror("listen: ");
		close(sockfd);
		return 0;
	}
	fd_set readfds;
	while(run) {
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);
		// use list to hold connfds
		// loop through it
		if(connfd != -1)
			FD_SET(connfd, &readfds);
		int r = select(sockfd>connfd ? sockfd + 1 : connfd + 1, &readfds, NULL, NULL, NULL);
		if ( r == -1)
			run = 0;
		if(r > 0 && FD_ISSET(sockfd, &readfds))
			// add to list
			connfd = accept(sockfd, NULL, NULL);
		// loop through list of connfd
		if(r > 0 && connfd != -1 && FD_ISSET(connfd, &readfds)) {
			char in[1000];
			memset(in, '\0', 1000);
			read(connfd, in, 1000);
			printf("%s", in);
			if(strncmp(in,"quit",4) == 0)
				run = 0;
		}
	}
	printf("Closing sockets\n");
	close(connfd);
	close(sockfd);
}