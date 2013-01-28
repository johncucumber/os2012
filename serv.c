#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>



#define sockSize 3

void *start();
void put(int sock1);
void my_get(char *filename, int sock);
int sockets[sockSize];
int listener;
struct sockaddr_in addr;
char buf[1024];
int bytes_read;
int working = 0;
sem_t sem[3];
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;


int main()
{
    int i;
    int sock;
    for (i = 0; i<sockSize; i++)
    {
	sockets[i] = 0;
	pthread_t thread;
	sem_init(&sem[i], 0, 0);
	pthread_create (&thread, NULL, start, i);
    }
    sem_init(&sem, 0, 0);
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }

    printf("hgskadfa\n");fflush(stdout);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1555);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, SOMAXCONN);
    printf("hgskadfa2\n");fflush(stdout);
    while(1)
    {
	printf("hgskadfa3\n");fflush(stdout);	;
        sock = accept(listener, NULL, NULL);
	printf("got client\n");fflush(stdout);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
	printf("iiiii\n");fflush(stdout);
	if (working<sockSize)
	{
	pthread_mutex_lock(&mymutex);
	put(sock);
        printf("iiiii2\n");fflush(stdout);
	printf("%d\n", working);
	fflush(stdout);
	working = working + 1;
	printf("%d\n", working);
	fflush(stdout);
        pthread_mutex_unlock(&mymutex);

	
	sem_post(&sem[working]);
	}
	else 
	{
		strcpy(buf, "goodbye");
		printf("server full\n");
		fflush(stdout);
		send(sock, buf, sizeof (buf), 0);
		//working--;
	}
	
    }
    
    return 0;
}

void *start(void *arg)
{
	int j = (int) arg;
 	while(1)
        {
	    sem_wait(&sem[j]);
	    pthread_mutex_lock(&mymutex);
            int sock1 = pop();
            pthread_mutex_unlock(&mymutex);
	    if (sock1 != 0)
	    {


		 int count = 1;
		 int count1;
                /* for (int j = 0; j<=64000; j++)
		 {
			count++;
                 }  */
		fd_set myset;
		FD_ZERO (&myset);
		FD_SET(sock1, &myset); 

		/*struct timeval {
    			int tv_sec;     // секунды
    			int tv_usec;    // микросекунды
		};
		timeval timeout;
                timeout.tv_sec = 15;
                timeout.tv_usec = 0;*/
		printf("shas poidet!");
		fflush(stdout);
		bytes_read = 1;
		while ((count != 0) && (bytes_read != 0 ))
		{
			count++;
			//printf ("%s", bytes_read);
			printf("%d \n", count);
			fflush(stdout);
                        bytes_read = recv(sock1, buf, 1024, 0);
			
                }
		printf("proshlo!");
		fflush(stdout);
		working--;
           	 bytes_read = recv(sock1, buf, 1024, 0);
		 if (bytes_read > 0) 
		 {
                     my_get(buf, sock1);
                 }
	   	// send(sock1, buf, bytes_read, 0);
	    }
        close(sock1);
        }
	printf(working);
	fflush(stdout);
	
}

void put(int sock1)
{
	int i = 0;
	for (i = 0; i<sockSize; i++)
	{
		if (sockets[i] == 0)
		{
			sockets[i] = sock1;
		}
	}
}

int pop()
{
	int tmp;
	int i;
	for (i = 0; i < sockSize; i++)
	{
		if (sockets[i] != 0)
		{
			tmp = sockets[i];
			sockets[i] = 0;
		}
	}
	return tmp;
}



void my_get(char *reqest, int sock) {   
    char delims[] = " \n";
    char *command = strtok(reqest, delims);
    char *filename = strtok(NULL, delims);
    char *protocol = strtok(NULL, delims);

    printf("Get request \"%s %s %s\" \n", command, filename, protocol, fflush);

    if (strcmp(command, "GET")) {
        printf("Non GET \n", fflush);
        return;
    }

    char def[] = "index.html";

    if (!strcmp(filename, "/"))
        filename = def;
    else
        filename = filename + 1;
    printf("Filename %s \n", filename, fflush);

    FILE *file;
    if ((file = fopen(filename, "r")) == NULL) {
        printf("404 Not Found\n", fflush);
        char header[] = "HTTP/1.1 404 Not Found\nServer: dimko/0.0.1\nContent-Type: text/html\nConnection: close\n\n";
        send(sock, header, sizeof (header), 0);
    } else {
        printf("200 OK\n", fflush);
        char header[] = "HTTP/1.1 200 OK\nServer: dimko/0.0.1\nContent-Type: text/html; charset=utf-8\nConnection: close\n\n";
        send(sock, header, sizeof (header), 0);
        char buf[1024];
        char c;
        int i = 0;
        printf("Start send file\n", fflush);
        memset(buf, 0, sizeof (buf));
        while ((c = fgetc(file)) != EOF) {
            buf[i] = c;
            i++;
            if ((i == 1024 - 1)) {
                send(sock, buf, sizeof (buf), 0);
                i = 0;
                memset(buf, 0, sizeof (buf));
            }
        }
        if (i != 0) {
            send(sock, buf, sizeof (buf), 0);
        }
        printf("File was sended\n", fflush);
        fclose(file);
    }
}
