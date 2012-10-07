#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *Sort();
void *Sort2();
void *Sort3();
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int count = 0;
//int Arr[10];
struct 
{
	int Arr[10];
	int size;
} MyStruct, MyStruct1;


void main()
{
int j;
int size = 10;
for (j = 0; j<=9; j++)
{
//	size++;
	MyStruct.Arr[j] = 0 +  rand() %50;
}
int p;
MyStruct.size = size;
for (p = 0; p<=size-1; p++)
{
	printf("%d   ",MyStruct.Arr[p]);
}
printf("\n");
int i;
int iret1, iret2, iret3;
pthread_t thread1;
pthread_t thread2;
pthread_t thread3;

iret1 = pthread_create( &thread1, NULL, Sort2, NULL);
iret2 = pthread_create( &thread2, NULL, Sort, NULL); 



pthread_join( thread1, NULL);
pthread_join( thread2, NULL);

for (p = 0; p<=size-1; p++)
{
	printf("%d   ", MyStruct.Arr[p], size);
}
printf("\n");

iret3 = pthread_create( &thread3, NULL, Sort3, NULL); 
pthread_join( thread3, NULL);
for (p = 0; p<=size-1; p++)
{
	printf("%d   ", MyStruct.Arr[p], size);
}

}

//void *Sort(void *arg)
void *Sort()
{

//MyStruct1 = (MyStruct) arg;

int l;
int k;
int key;

for (k= 0; k<5; k++)
{
key = MyStruct.Arr[k];
l = k - 1;
while (l>=0 && MyStruct.Arr[l] > key )
{
	MyStruct.Arr[l+1] = MyStruct.Arr[l];
	l--;
}
MyStruct.Arr[l+1]= key;
}


}

void *Sort2()
{

//MyStruct1 = (MyStruct) arg;

int l;
int k;
int key;

for (k= 5; k<MyStruct.size; k++)
{
key = MyStruct.Arr[k];
l = k - 1;
while (l>=5 && MyStruct.Arr[l] > key )
{
	MyStruct.Arr[l+1] = MyStruct.Arr[l];
	l--;
}
MyStruct.Arr[l+1]= key;
}


}

void *Sort3()
{

//MyStruct1 = (MyStruct) arg;

int l;
int k;
int key;

for (k= 0; k<MyStruct.size; k++)
{
key = MyStruct.Arr[k];
l = k - 1;
while (l>=0 && MyStruct.Arr[l] > key )
{
	MyStruct.Arr[l+1] = MyStruct.Arr[l];
	l--;
}
MyStruct.Arr[l+1]= key;
}


}

/*void *Sort2()
{
int l;
int k;
int p;
int tmp;
for (k= MyStruct.size-1; k>=1; k--)
{
for (l = MyStruct.size-1; l >= MyStruct.size - k; l--  )
{
pthread_mutex_lock(&mutex);
	if (MyStruct.Arr[l] > MyStruct.Arr[l-1])
	{
		tmp = MyStruct.Arr[l];
		MyStruct.Arr[l]= MyStruct.Arr[l-1];
		MyStruct.Arr[l-1] = tmp;
	}
pthread_mutex_unlock(&mutex);
}
}
}
*/
