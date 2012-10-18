	
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

void* ClientServ(void *arg)
{
	int **tmp = (int **)arg;
	int *pipes = *tmp;
	message msg;
	while (1)
	{
		read(pipes[0], &msg, sizeof(msg));
		//read(pipes[0], &fd, sizeof(int));
		char buf;
		read (msg.fd, &buf, sizeof buf);
		printf("cl = %c\n", buf);
		buf++;
		write (msg.fd, &buf, sizeof buf);
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
