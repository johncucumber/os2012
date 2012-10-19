	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <pthread.h>

#include "taskmanager.h"
#include "stringwork.h"

char *login = "admin";
char *pass = "123";



void SSHWork(int pipes[2], int fd)
{
	//char buf;
	//read (fd, &buf, sizeof buf);
	//printf("cl = %c\n", buf);
	//buf++;
	int pid = pthread_self();
	char* buf = readstring(fd);
	printf("login = %s %d\n", buf, pid);
	if (!strcmp(buf, login))
	{
		printf("login was rec\n");
	}
	buf = readstring(fd);
	printf("pass = %s %d\n", buf, pid);	
	if (!strcmp(buf, pass))
	{
		printf("pass was rec\n");
		writestring(fd, "ok");
	}	
	else
	{
			writestring(fd, "no");
			return ;
	}
	while (strcmp(buf, "exit"))
	{
		buf = readstring(fd);
		
		printf("command = %d %d\n", strlen(buf), pid);			
		//writestring(fd, "Hi!");
	}
	printf("rec exit\n");
	//sleep(1);
	//write (fd, &buf, sizeof buf);
}

void* ClientServ(void *arg)
{
	int **tmp = (int **)arg;
	int *pipes = *tmp;
	message msg;
	while (1)
	{
		read(pipes[0], &msg, sizeof(msg));
		//read(pipes[0], &fd, sizeof(int));
		SSHWork(pipes, msg.fd);
		printf ("Closed connection on descriptor %d\n", msg.fd);
		close (msg.fd);
	}
}

int main (int argc, char *argv[])
{

	if (argc != 3)
	{
		printf("\nUSAGE: server <port> <num_of_threads>\n");
		return 0;
	}
	
	int num_of_threads = atoi(argv[2]);
	int pipes[2];
	pipe(pipes);
	createThreads(num_of_threads, pipes, ClientServ);
	startlistning(argv[1], pipes);

	return EXIT_SUCCESS;
}
