#include<signal.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

/*
ANSI C:
void handler(int);

POSIX SA_SIGINFO:
void handler(int, siginfo_t *info, ucontext_t *uap);
*/

/*
struct  sigaction {
	union __sigaction_u __sigaction_u;  // signal handler 
    sigset_t sa_mask;               // signal mask to apply 
    int     sa_flags;               // see signal options below 
};

union __sigaction_u {
	void    (*__sa_handler)(int);
	void    (*__sa_sigaction)(int, siginfo_t *, void *);
};

#define sa_handler      __sigaction_u.__sa_handler
#define sa_sigaction    __sigaction_u.__sa_sigaction

int sigaction(int sig, const struct sigaction *restrict act, struct sigaction *restrict oact);
*/

void myhandler(int code, siginfo_t * info, void * uap){
	printf("Caputured SIGINT, exiting now..\n");
	//exit(0);
}

void myhandler2(int code) {
	printf("Caputured SIGINT, exiting now..\n");
	//exit(0);
}

int main() {
	int status;
	struct sigaction mysigaction;
	memset(&mysigaction, 0 , sizeof(struct sigaction));
	mysigaction.sa_flags = SA_SIGINFO;
	mysigaction.__sigaction_u.__sa_sigaction = &myhandler;
	//mysigaction.__sigaction_u.__sa_handler = &myhandler2;
	
	status = sigaction(SIGINT, &mysigaction, NULL);
	while(1) {
		sleep(1);
		printf(".\n");
	}
	return 0;
}