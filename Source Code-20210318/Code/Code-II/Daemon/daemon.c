#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>

#define MAXFD 64
int main() {
	int i;
	pid_t pid;
	if((pid = fork()) < 0) 
		return -1;
	else if (pid)
		_exit(0);

	if(setsid() < 0)
		return -1;

	signal(SIGHUP, SIG_IGN);

	if ((pid = fork()) < 0)
		return -1;
	else if (pid)
		_exit(0);

	chdir("/");

	for(i = 0; i < MAXFD; i++)
		close(i);

	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	/*
	while(1) {
	// Do something
	}
	*/
	openlog("MyDaemon: ", LOG_PID, LOG_USER);
	syslog(LOG_ERR, "Log message from process with id %d",getpid());
	closelog();
	return 0;
}