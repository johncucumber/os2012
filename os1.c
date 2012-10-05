#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *Sort(int[], int);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int arr[21];
int ch1[10],ch2[11];
int k,p1,p2,s1,s2;

int main()
{ int i;
 int s=0;
 for (i=1; i<=21; i++)
 {
   arr[i]=0+rand() %50;;
   s++;
 }
 for (i=1; i<=(10); i++)
 {  s1=0;
   ch1[i]=arr[i];
   s1++;
 }
 for (i=(10); i<=11; i++)
 { s2=0;
   ch2[i-9]=arr[i];
   s2++;
 }
pthread_t thread1;
pthread_t thread2;
p1 = pthread_create(&thread1,NULL,Sort(ch1,s1), NULL);
p2 = pthread_create(&thread2,NULL,Sort(ch2,s2), NULL);

printf("Vivod");
for (k = 1; k<=s; k++)
{
  printf(arr[k]);
}
}
void *Sort(int arr[],int col)
{
   int i,j,buf=0;             
   for (i=1; i<=col; i++)             
    {
        for (j=1; j<=col-i; j++)       
        {
pthread_mutex_lock(&mutex);
            if (arr[j]>arr[j+1])      
            {
                buf=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=buf;
            }
pthread_mutex_unlock(&mutex);
        }
    }
}


