#ifndef taskmanagerH
#define taskmanagerH

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
#include "message.h"

typedef void* (*clientserv_f)(void *data);

void createThreads(int num_of_threads, int pipes[2], clientserv_f func)
{
	pthread_t *thread = new pthread_t[num_of_threads];	
	printf("before thread create\n");
	for (int i = 0; i < num_of_threads; i++)
	{
		pthread_create(&thread[i], NULL, func, (void*)&pipes);
	}
	printf("created %d threads\n", num_of_threads);

}

#endif
