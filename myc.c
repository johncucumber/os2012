#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *Sort(int[], int);
void *Sort2(int[], int);
FILE *file;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int count = 0;
int Arr[];
char *fname = "/home/mike/github/os2012/array";

void main()
{
int j;
int size = -1;
file = fopen(fname, "r");
if(file == 0)
        {
                printf("не могу открыть файл '%s'",fname);
                return 0;
        }
for (j = 0; j<=10; j++)
{
	size++;
	Arr[size] = 0 +  rand() %50;
}
int i;
int iret1, iret2;
pthread_t thread1;
pthread_t thread2;
iret1 = pthread_create( &thread1, NULL, Sort2(Arr, size), NULL);
iret2 = pthread_create( &thread2, NULL, Sort(Arr, size), NULL); 
/*pthread_t thread1;
pthread_t thread2;
iret1 = pthread_create( &thread1, NULL, Sort, NULL);
iret2 = pthread_create( &thread2, NULL, Sort, NULL); 

for (i = 0; i < 100; i++)
{
     iret1 = pthread_create( &thread1, NULL, Sort, NULL);
     iret2 = pthread_create( &thread2, NULL, Sort, NULL); 
     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL);
     printf("%d \n", count);
}*/
int p;
for (p = 0; p<=size; p++)
{
	printf("%d   ", Arr[p], size);
}
	

}

void *Sort(int Arr[],int size)
{
int l;
int k;
int p;
int tmp;
for (p = 0; p<=size; p++)
{
	printf("%d   ", Arr[p]);
}
printf("\n");
for (k= 0; k<=size; k++)
{
for (l = 0; l <= size - k; l++  )
{
pthread_mutex_lock(&mutex);
	if (Arr[l] < Arr[l+1])
	{
		tmp = Arr[l];
		Arr[l]= Arr[l+1];
		Arr[l+1] = tmp;
	}
pthread_mutex_unlock(&mutex);
}
}
//pthread_mutex_lock(&mutex);
//pthread_mutex_unlock(&mutex);

}

void *Sort2(int Arr[],int size)
{
int l;
int k;
int p;
int tmp;
for (p = 0; p<=size; p++)
{
	printf("%d   ", Arr[p]);
}
printf("\n");
for (k= size; k>=1; k--)
{
for (l = size; l >= k; l--  )
{
pthread_mutex_lock(&mutex);
	if (Arr[l] > Arr[l-1])
	{
		tmp = Arr[l];
		Arr[l]= Arr[l-1];
		Arr[l-1] = tmp;
	}
pthread_mutex_unlock(&mutex);
}
}
//pthread_mutex_lock(&mutex);
//pthread_mutex_unlock(&mutex);
}
