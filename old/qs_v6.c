#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define n 20
int s_arr[n];	

int thReady=0;
int thCount=0;

pthread_mutex_t lockMain = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct Params //передаю значения в метод QS
{
	int first;
	int last;
};

void swap(int *a,int *b);
void *start_qs(void *ptr);
void *qs(int first, int last);
int main(void)
{//100000000
	int i;
	srand(time(NULL));
	for(i=0;i<n;i++) s_arr[i]=rand()%100;	
	//for(i=0;i<n;i++) printf("%d ", s_arr[i]);
	
	struct Params p = {0, n-1};	
	thCount++;	
	pthread_t th;
	pthread_create(&th, NULL, (void*)start_qs, (void*) &p);
		
	while (thReady!=thCount)
		pthread_cond_wait(&cond, &lockMain);
		
	printf("\n");for(i=0;i<n;i++) printf("%d ", s_arr[i]);
	return 0;
}


void *start_qs(void *ptr)
{
	//printf("istarted %d %d\n", thReady,thCount);fflush(stdout);
	struct Params *p;
	p = (struct Params *) ptr;//читаем переданные параметры	
	qs(p->first, p->last);
	thReady++;
	pthread_cond_signal(&cond);
    return 0;
}

void *qs(int first,int last)
{
    int i = first, j = last, x = s_arr[(first + last) / 2];
 
    do {
        while (s_arr[i] < x) i++;
        while (s_arr[j] > x) j--;
 
        if(i <= j) {
            if (i < j) swap(&s_arr[i], &s_arr[j]);
            i++;
            j--;
        }
    } while (i <= j);	
	
    if (i < last)
    {
		struct Params p = {i, last};	
		thCount++;	pthread_t th; 
		pthread_create(&th, NULL, (void*)start_qs, (void*) &p);
		//pthread_join(th,NULL);      если раскомментируем то программа не падает, но время выполнения становится огромным		
    }
    
    if (first < j)
    {
		qs(first, j);
    }	
    
    return 0;
}

void swap(int *a,int *b)
{
	int t;
	t = *a, *a = *b, *b = t;
}

