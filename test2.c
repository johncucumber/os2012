#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#define th_count 3

pthread_t array_of_threads [th_count];
pthread_mutex_t array_of_mutex [th_count];
pthread_cond_t array_of_cond [th_count];

		
void *th(int *in);
void *th(int *in)
{ 	
	int n = in; 
	printf("Я создался №%d\n" , n);fflush(stdout);	
	sleep(1);
	
	pthread_cond_wait(&(array_of_cond[n]), &(array_of_mutex[n]));
	printf("Я разлочен №%d\n" , n);fflush(stdout);	
	
}


int main(void)
{	
	int index;
	
	for(index=0;index<th_count;index++)
	{
        pthread_mutex_init(&(array_of_mutex[index]), NULL);
        pthread_cond_init(&(array_of_cond[index]), NULL);
        pthread_create(&(array_of_threads[index]), NULL, th, index); 
	}
	getchar();
	//if(array_of_mutex[1]==1))
	pthread_cond_broadcast(&array_of_cond[1]);
	getchar();
}



