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
#include <fcntl.h>

int main() {
	int sockfd1, sockfd2;
	struct sockaddr_in remote1,remote2;

	memset(&remote1, 0, sizeof(struct sockaddr_in));
	remote1.sin_family = AF_INET;
	remote1.sin_port = htons(80);
	inet_pton(AF_INET,"127.0.0.1",&remote1.sin_addr.s_addr);

	memset(&remote2, 0, sizeof(struct sockaddr_in));
	remote2.sin_family = AF_INET;
	remote2.sin_port = htons(80);
	inet_pton(AF_INET,"127.0.0.1",&remote2.sin_addr.s_addr);


	sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
	sockfd2 = socket(AF_INET, SOCK_STREAM, 0);

	// set socket in non-blocking mode
	int flags;
	flags = fcntl(sockfd1, F_GETFL);
	flags = flags | O_NONBLOCK;
	fcntl(sockfd1, F_SETFL, flags);

	flags = fcntl(sockfd2, F_GETFL);
	flags = flags | O_NONBLOCK;
	fcntl(sockfd2, F_SETFL, flags);


	connect(sockfd1, (struct sockaddr *)&remote1, sizeof(struct sockaddr_in));
	connect(sockfd2, (struct sockaddr *)&remote2, sizeof(struct sockaddr_in));


	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(sockfd1, &writefds);
	FD_SET(sockfd2, &writefds);
	select(sockfd1>sockfd2?sockfd1+1:sockfd2+1,NULL,&writefds,NULL,NULL);
	if(FD_ISSET(sockfd1,&writefds)) {
		int err;
		socklen_t len = sizeof(int);
		getsockopt(sockfd1,SOL_SOCKET,SO_ERROR, &err, &len);
		if(err == 0) {
			char out[] ="THIS IS THE MESSAGE FOR #1\n";
			write(sockfd1, out, strlen(out));
			close(sockfd1);
		}
	}
	if(FD_ISSET(sockfd2,&writefds)) {
		int err;
		socklen_t len = sizeof(int);
		getsockopt(sockfd2, SOL_SOCKET, SO_ERROR, &err, &len);
		if(err == 0) {
			char out[] ="THIS IS THE MESSAGE FOR #2\n";
			write(sockfd2, out, strlen(out));
			close(sockfd2);
		}
	}
}