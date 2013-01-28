#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#define myPort 1488
#define thCount 3
#define BUF_SIZE 512
int arrOfSockets[thCount];
int workingThr=0;
sem_t mySem;
pthread_mutex_t myMut = PTHREAD_MUTEX_INITIALIZER;

void *push(int _sock)
{
	int i;
	for (i = 0; i < thCount; i++)  {
		if(arrOfSockets[i]==0)
		{
			arrOfSockets[i]=_sock;
			break;
		}
	}
}

int pop()
{
	int i,tmp=0;
	for (i = 0; i < thCount; i++) {
		if(arrOfSockets[i]!=0)
		{
			tmp=arrOfSockets[i];	
			arrOfSockets[i]=0;		
			break;
		}
	}
	return tmp;
}

_Bool check()
{
	int i;
	for (i = 0; i < thCount; i++)  {
		if(arrOfSockets[i]==0)
			return 1;		
	}
	return 0;
}

char *retString(char *header)
{
	// GET /1.txt HTTP/1.0
char result_sting[BUF_SIZE];

char *token;char *filename;

token = strtok(header," ");
if(strcmp( token, "GET") == 0)
{
	token = strtok(NULL," ");
	filename = strtok( token, "/" );
}
   //printf("%s\n", filename);
    FILE *file;
    file = fopen(filename,"r");
    
    if (fgets(result_sting,BUF_SIZE,file))
    {
		printf("retStringOk ");fflush(stdout);
		return result_sting;
	}
}


void *thrWorking()
{
	while (1) 
	{
		sem_wait(&mySem);
		pthread_mutex_lock(&myMut);printf("lock ");fflush(stdout);
		int sock= pop();
		printf("\npop sock=%i\n",sock);fflush(stdout);
		pthread_mutex_unlock(&myMut);printf("unlock ");fflush(stdout);
		int bytes_read;
		char buf[BUF_SIZE];
		
		while(1)
		{
			bytes_read = recv(sock, buf, BUF_SIZE, 0);
			if(bytes_read<=0)
			{
				if(bytes_read==0) break;
				if(bytes_read==-1) printf("\nCLIENT ERROR\n");fflush(stdout);
			}else{
				if(send(sock, retString(buf), BUF_SIZE, 0)==-1)
					printf("\nCLIENT ERROR\n");fflush(stdout);
			}
		}
	
		pthread_mutex_lock(&myMut);printf("lock ");fflush(stdout);
		workingThr--;
		close(sock);
		printf("client disconnected\n");fflush(stdout);
		pthread_mutex_unlock(&myMut);printf("unlock ");fflush(stdout);
	}
}


int main()
{
    int sock, listener,i;
    struct sockaddr_in addr;
    sem_init(&mySem, 0, 0);
    
    for (i = 0; i < thCount; i++) {
		arrOfSockets[i]=0;
		pthread_t thread;
		pthread_create(&thread, NULL, thrWorking, NULL );
	}
    
    
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(myPort);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, SOMAXCONN);
    
    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
        pthread_mutex_lock(&myMut); printf("lock ");fflush(stdout);
        if(check())//проверим есть ли свободные потоки
			{
				push(sock);
				sem_post(&mySem);workingThr++;// число работающих++
				printf("\npost sock=%i\n",sock);fflush(stdout);
			}
		else printf("\nfull\n");fflush(stdout);
		pthread_mutex_unlock(&myMut);printf("unlock ");fflush(stdout);
    }
    
    return 0;
}
