	
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

#include "listner.h"


int pipes[2];


void* ClientServ(void *arg)
{
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
	pipe(pipes);
	pthread_t *thread = new pthread_t[num_of_threads];
	
	printf("before thread create\n");
	for (int i = 0; i < num_of_threads; i++)
	{
		pthread_create(&thread[i], NULL, ClientServ, NULL);
	}
	printf("after thread create\n");

	startlistning(argv[1], pipes);

	return EXIT_SUCCESS;
}
