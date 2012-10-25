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

int fdtable[sizeof (int)];

void SSHWork(int pipes[2], int fd)
{
	int pid = pthread_self();
	char buf[512];
	read(fd, buf, sizeof buf);
	printf("%d:reciv = %s\n", pid, buf);
	//buf = "good boy\n";
	write(fd, "good", 5);
	sleep(1);
	read(fd, buf, sizeof buf);
	printf("%d:reciv2 = %s\n", pid, buf);
	write(fd, "good2", 6);
}

void* ClientServ(void *arg)
{
	int **tmp = (int **)arg;
	int *pipes = *tmp;
	message msg;
	while (1)
	{
		read(pipes[0], &msg, sizeof(msg));
		int pid = pthread_self();
		printf("read task in %d fd = %d\n", pid, msg.fd);
		if (fdtable[msg.fd] != 0)
		{
			printf("reject fd %d\n", msg.fd);
		}
		else
		{
			fdtable[msg.fd] = 1;
			SSHWork(pipes, msg.fd);
			fdtable[msg.fd] = 0;			
			printf ("%d:Closed connection on descriptor %d\n", pid, msg.fd);
			close (msg.fd);
		}		
	}
}

int main (int argc, char *argv[])
{

	if (argc != 3)
	{
		printf("\nUSAGE: server <port> <num_of_threads>\n");
		return 0;
	}
	for (int i = 0; i < sizeof(int); i++)
	{
		fdtable[i] = 0;
	}
	int num_of_threads = atoi(argv[2]);
	int pipes[2];
	pipe(pipes);
	createThreads(num_of_threads, pipes, ClientServ);
	startlistning(argv[1], pipes);

	return EXIT_SUCCESS;
}


/*
	char* buf = readstring(fd);
	printf("login = %s %d\n", buf, pid);
	if (!strcmp(buf, login))
	{
		printf("login was rec\n");
	}

	read(fd, buf2, sizeof buf);
	printf("buf2 = %s\n", buf2);
	//buf = readstring(fd);
	printf("pass = %s %d\n", buf, pid);	
	if (!strcmp(buf, pass))
	{
		printf("pass was rec\n");
		//writestring(fd, "ok");
	}	
	else
	{
			//writestring(fd, "no");
			return ;
	}
	
	buf = readstring(fd);	
	printf("command = %s %d\n", buf, pid);
	buf = readstring(fd);	
	printf("command = %s %d\n", buf, pid);				
	/*
	while (strcmp(buf, "exit"))
	{
		
		//writestring(fd, "Hi!");
	}*/
//	printf("rec exit\n");
	//sleep(1);
	//write (fd, &buf, sizeof buf);


