#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *Sort(void *arg);
void *Sort2();
void *Sort3();
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int count = 0;
//int Arr[10];
struct mystruct
{
	int Arr[10];
	int size;
	int low;
	int high;
};


void main()
{
int j;
int size = 10;

struct mystruct MyStruct, MyStruct1;

for (j = 0; j<=9; j++)
{
//	size++;
	MyStruct.Arr[j] = 0 +  rand() %50;
}



int p;
MyStruct.size = size;
MyStruct.low = 0; 
MyStruct.high = 5;

MyStruct1 = MyStruct;
MyStruct1.low = 5;
MyStruct1.high = MyStruct.size;

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


iret1 = pthread_create( &thread1, NULL, Sort, &MyStruct);

iret2 = pthread_create( &thread2, NULL, Sort, &MyStruct1); 



pthread_join( thread1, NULL);
pthread_join( thread2, NULL);

for (p=4; p<=9; p++)
{
	MyStruct.Arr[p]=MyStruct1.Arr[p];
}

for (p = 0; p<=size-1; p++)
{
	printf("%d   ", MyStruct.Arr[p], size);
}
printf("\n");

MyStruct.low = 0;
MyStruct.high = MyStruct.size;

iret3 = pthread_create( &thread3, NULL, Sort, &MyStruct); 
pthread_join( thread3, NULL);
for (p = 0; p<=size-1; p++)
{
	printf("%d   ", MyStruct.Arr[p], size);
}

}

void *Sort(void *arg)
{
struct mystruct *MyStruct1 = (struct mystruct *) arg;
int l;
int k;
int key;

for (k= MyStruct1->low; k<MyStruct1->high ; k++)
{
key = MyStruct1->Arr[k];
l = k - 1;
while (l>=MyStruct1->low && MyStruct1->Arr[l] > key )
{
	MyStruct1->Arr[l+1] = MyStruct1->Arr[l];
	l--;
}
MyStruct1->Arr[l+1]= key;
}


}

/*
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
