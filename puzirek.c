#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *Sort_ch1();
void *Sort_ch2();
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct
{
int Arr[10];
int size;
} OneStruct;

struct
{
int Arr[5];
int size;
} ch1;

struct
{
int Arr[5];
int size;
} ch2;

void main()
{
int i;
int size=10;
for (i=0; i<=9; i++)
{
  OneStruct.Arr[i]=0+rand() %20;
}

OneStruct.size=size;
for (i=0; i<=size-1;i++)
{
 printf("%d    ", OneStruct.Arr[i]);
}
printf("\n");
printf("ch 1");
ch1.size=size/2;
printf("\n");
for (i=0; i<=4;i++)
{
 printf("%d    ", ch1.Arr[i]=OneStruct.Arr[i]);
}
printf("\n");
printf("ch 2");
printf("\n");
ch2.size=size/2;
for (i=5; i<=9;i++)
{
 printf("%d    ", ch2.Arr[i]=OneStruct.Arr[i]);
}
printf("\n");

int p1,p2;
pthread_t thread1;
pthread_t thread2;

p1=pthread_create(&thread1,NULL,Sort_ch1(),NULL);
p2=pthread_create(&thread2,NULL,Sort_ch2(),NULL);

pthread_join( thread1, NULL);
pthread_join( thread2, NULL);
}
void *Sort_ch1()
{

int i;
int j;
int buf;

for (i= 0; i<=ch1.size; i++)
{
for (j = ch1.size - 1; j > i; j--) 
{
pthread_mutex_lock(&mutex);
if (ch1.Arr[j] < ch1.Arr[j+1])
{
buf = ch1.Arr[j];
ch1.Arr[j]= ch1.Arr[j+1];
ch1.Arr[j+1] = buf;
}
pthread_mutex_unlock(&mutex);
}
}
for (i=0; i<=ch1.size;i++)
{
 printf("%d    ", ch1.Arr[i]);
}
}


void *Sort_ch2()
{

int i;
int j;
int buf;

for (i= 0; i<=ch2.size; i++)
{
for (j = ch1.size - 1; j > i; j--) 
{
pthread_mutex_lock(&mutex);
if (ch2.Arr[j] < ch2.Arr[j+1])
{
buf = ch1.Arr[j];
ch2.Arr[j]= ch2.Arr[j+1];
ch2.Arr[j+1] = buf;
}
pthread_mutex_unlock(&mutex);
}
}
for (i=0; i<=ch2.size;i++)
{
 printf("%d    ", ch2.Arr[i]);
}
}


