#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#define thCount 4
pthread_t th;
pthread_mutex_t lockMain = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int blocker=0;

#define length 100000000
int s_arr[length];//массив
	
void *start_qs(void *ptr);
void *qs(int first,  int last);
void swap(int *a,int *b);
struct Params //передаю значения в метод QS
{
	int first;
	int last;
};

int main(int argc, char** argv)
{
	int s,i;
	//int th_count=thCount;
	int th_count=atoi(argv[1]);
	srand(time(NULL ));
	
	//создал массив
	for(i=0;i<length;i++) s_arr[i]=rand()%100;
	//for(i=0;i<length;i++) printf("%d ", s_arr[i]);printf("\n");
	
	struct Params par[th_count];
	
	
	th_count=th_count*2;
	do
	{
		th_count=th_count/2;//необходимо чтобы цикл запустился со счетчиком=1

		s=length/th_count; //длина части массива обрабатываемого 1  потоком
		for(i=0; i<th_count;i++)
		{
			par[i].first=i*s;
			par[i].last=i*s+s;
			if (i == (th_count - 1))
				par[i].last=(length-1);
			//потоки создаем-с
			pthread_create(&th, NULL, start_qs, (void*) &par[i]);
		}		
		while (blocker!=th_count)
			pthread_cond_wait(&cond, &lockMain);
		blocker = 0;
	}while (th_count!=1);
	
	
	//вывод массива
//	printf("\n");
//	for(i=0;i<length;i++) printf("%d ", s_arr[i]);//Вывод масссива
	return 0;
}

void *start_qs(void *ptr)
{
	struct Params *p;
	p = (struct Params *) ptr;
	printf("%d %d ",p->first,p->last);fflush(stdout);
	qs(p->first,p->last);	
	
	pthread_mutex_lock(&lockMain);	
	blocker++;		
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&lockMain);
    return 0;
}

void *qs(int first,  int last) //сортировка массива
{
	int i = first, j = last, x = s_arr[(first + last) / 2];
	do
	{
	while (s_arr[i] < x) i++;
	while (s_arr[j] > x) j--;
		if(i <= j)
		{
			if (i < j) swap(&s_arr[i], &s_arr[j]);//раскидываем в разные стороны от осевого элемента
			i++;
			j--;
		}
	}
	while (i <= j);

	//повторяем рекурсивно
	if (i < last)
	{
		qs(i,last);
	}
	if (first < j)
	{
		qs(first,j);
	}
	return 0;
}

void swap(int *a,int *b)
{
	int t;
	t = *a, *a = *b, *b = t;
}

