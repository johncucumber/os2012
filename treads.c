#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
sem_t sem;

struct
{
  int num;
  int sum;
} temp;

void * thread_func(void *arg)
{ 
while (temp.num<13){
sem_post(&sem);
int loc_id = * (int *) arg;
printf("Running threads %d => Num = %d, Sum = %d\n", loc_id, temp.num, temp.sum);
temp.num++;
temp.sum*=temp.num;
sleep(rand()%3);}
}
int main(int argc, char * argv[])
{ 
int id1, id2, result;
temp.num=1;
temp.sum=1;
pthread_t thread1, thread2;
sem_init(&sem, 0, 0);
id1=1;
result = pthread_create(&thread1, NULL, thread_func, &id1);
if (result != 0) {
perror("Creating the first thread");
return EXIT_FAILURE;
}
id2=2;
result = pthread_create(&thread2, NULL, thread_func, &id2);
if (result != 0) {
perror("Creating the second thread");
return EXIT_FAILURE;
}
result = pthread_join(thread1, NULL);
if (result != 0) {
perror("Joining the first thread");
return EXIT_FAILURE;
}
result = pthread_join(thread2, NULL);
if (result != 0) {
perror("Joining the second thread");
return EXIT_FAILURE;
}
sem_destroy(&sem);
printf("Done\n");
return EXIT_SUCCESS;
}
