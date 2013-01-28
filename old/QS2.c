#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define n 100000000
int s_arr[n];	
int blocker=0;

pthread_t th1;
pthread_t th2;
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
{
	int i;
	srand(time(NULL));
	for(i=0;i<n;i++) s_arr[i]=rand()%100;
	
	struct Params p1 = {0, (n-1)/2};
	struct Params p2 = {((n-1)/2)+1, n-1};
	
	pthread_create(&th1, NULL, (void*)start_qs, (void*) &p1);
	pthread_create(&th2, NULL, (void*)start_qs, (void*) &p2);
		
	while (blocker!=1)
		pthread_cond_wait(&cond, &lockMain);
	qs(0,n-1);		
	//for(i=0;i<n;i++) printf("%d ", s_arr[i]);
	return 0;
}


void *start_qs(void *ptr)
{
	struct Params *p;
	p = (struct Params *) ptr;//читаем переданные параметры
	qs(p->first, p->last);
	blocker++;
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
        qs(i, last);
    if (first < j)
        qs(first,j);
    return 0;
}

void swap(int *a,int *b)
{
	int t;
	t = *a, *a = *b, *b = t;
}

