#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *count_func();
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int count = 0;

void main()
{
int i;
int iret1, iret2;
pthread_t thread1;
pthread_t thread2;
iret1 = pthread_create( &thread1, NULL, count_func, NULL);
iret2 = pthread_create( &thread2, NULL, count_func, NULL); 

for (i = 0; i < 100; i++)
{
     iret1 = pthread_create( &thread1, NULL, count_func, NULL);
     iret2 = pthread_create( &thread2, NULL, count_func, NULL); 
     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL);
     printf("%d \n", count);
}
	
}

void *count_func()
{
pthread_mutex_lock(&mutex);
     count++;
pthread_mutex_unlock(&mutex);

}

