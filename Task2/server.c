#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <semaphore.h>

#include "queue.h"

#define THC 2
#define PORT 1777
#define BUF_SIZE 256
sem_t queue_sem;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
struct Queue *queue;

void *performance();

int main()
{
    int sock, listener;
    sem_init(&queue_sem, 0, 0);
    struct sockaddr_in addr;
    queue = new_queue();
    
    int i;
    for (i = 0; i < THC; i++) {
		pthread_t thread;
		pthread_create(&thread, NULL, performance, NULL );
	}

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 1);
    
    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
        
        push(queue, sock);
        sem_post(&queue_sem);
    }
    
    return 0;
}

void *performance()
{
	while (1) {
		sem_wait(&queue_sem);
		pthread_mutex_lock(&mymutex);
		struct Node *param;
		param = pop(queue);
		pthread_mutex_unlock(&mymutex);
		
		int sock = param->sock;
		int bytes_read;
		char buf[BUF_SIZE];
		
		while(1)
        {
            bytes_read = recv(sock, buf, BUF_SIZE, 0);
            if(bytes_read <= 0) break;
            send(sock, buf, bytes_read, 0);
        }
    
        close(sock);
	}
}
