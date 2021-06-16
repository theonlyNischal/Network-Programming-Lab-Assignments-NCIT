#include <stdio.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdlib.h>

#include <unistd.h>

#include <errno.h>

#include <string.h>

#include <sys/types.h>

#include <time.h>

#define LISTENQ 10
#define MAXLINE 1024
#define MAXCLIENT 10
#define NAMELENGTH 10

// Global variables
int client_count = 0;
char global_message[MAXLINE];

typedef struct sockaddr SA;

void provide_registration(int roll) {
  FILE * file = fopen("users.txt", "r");
  char line[MAXLINE];
  int i = 0;
  int line_num = roll % 11;
  while (fgets(line, sizeof(line), file)) {
    if (i == line_num) {
      puts(line);
      //the global message is the registration.
      //the global message will be send to client
      bzero(global_message, MAXLINE);
      strcpy(global_message, line);

    }
    i = i + 1;
  }

  fclose(file);
}

int main(int argc, char ** argv) {
  int listenfd, connfd;
  int port;
  struct sockaddr_in servaddr, cliaddr;
  socklen_t cliaddr_len;
  int max_socket;
  fd_set reads, active_sockets;
  int s, n;
  char rbuffer[MAXLINE], buff[MAXLINE];
  char temp_id[MAXLINE];

  // Check for correct format: ./tcpInfoServer -p <port>
  // argc returns total number of argument.
  // if argc != 3, there might be some error.
  if (argc != 3) {
    perror("Usage: ./tcpInfoServer -p <port>");
    exit(0);
  }

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Cannot create socket.");
    exit(0);

  } // Creates a TCP Socket

  // argv is the array of arguments
  // argv[0] = ./tcpInfoServer
  // argv[1] = -p
  // argv[2] = <port>
  port = atoi(argv[2]); // ascii to integer conversion 

  bzero( & servaddr, sizeof(servaddr)); // fills servaddr with zeros.
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if (bind(listenfd, (SA * ) & servaddr, sizeof(servaddr)) < 0) {
    printf("Bind error.\n");
    exit(0);
  }
  printf("Server is waiting connection at port %d\t\n", port);
  if (listen(listenfd, LISTENQ) < 0) {
    printf("Listen error.");
    exit(0);
  }
  // active_sockets stores all of the active sockets.
  // right now only listenfd is active.
  FD_ZERO( & active_sockets);
  FD_SET(listenfd, & active_sockets);
  max_socket = listenfd; // max_socket holds the largest socket descriptor.

  while (1) {
    reads = active_sockets;
    s = select(FD_SETSIZE, & reads, 0, 0, NULL);
    if (s < 0) {
      printf("Error in Select Function.\n");
      exit(0);
    } else if (s == 0) {
      printf("Time Exceeded.\n");
    } else {
      for (int i = 0; i <= max_socket; i++) {
        //New Connection as listenfd is ready.
        if (FD_ISSET(i, & reads)) {
          if (i == listenfd) {
            cliaddr_len = sizeof(cliaddr);
            connfd = accept(listenfd, (SA * ) & cliaddr, & cliaddr_len); // accept an inbound connection
            if (connfd > 0) {
              printf("Connection from %s, port %d\n", inet_ntop(AF_INET, & cliaddr.sin_addr.s_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
              bzero(rbuffer, MAXLINE);
              // n = read(connfd, rbuffer, sizeof(rbuffer));
              n = recv(connfd, rbuffer, MAXLINE, 0);
              bzero(temp_id, MAXLINE);
              strcpy(temp_id, rbuffer);

              if (n < 0) {
                perror("Cannot read ID Number.\n");
                return 1;
              }

            } else {
              printf("Accept error.\n");
            }
            // Add new connection to active connection
            FD_SET(connfd, & active_sockets);
            add_client(connfd, rbuffer);
            provide_registration(atoi(temp_id));

            bzero(rbuffer, MAXLINE);
            sprintf(rbuffer, "%s", global_message);

            send(connfd, rbuffer, strlen(rbuffer), 0);
          }

        } else { // Is not new connection

          n = recv(i, rbuffer, MAXLINE, 0);
          if (n <= 0) {
            printf("%s left the Quiz.\n", "R");
            close(i);
            FD_CLR(i, & active_sockets);

          } else {
            printf("Received Message.\n");
            puts(rbuffer);
            if (strncmp(rbuffer, "2Again", 6) == 0) {
              provide_registration(atoi(temp_id));
              bzero(rbuffer, MAXLINE);
              sprintf(rbuffer, "%s", global_message);

              send(connfd, rbuffer, strlen(rbuffer), 0);

            }

          }

        }
      }

    } //

  } // while

}