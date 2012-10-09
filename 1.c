#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#define th_count 2
pthread_t array_of_threads[th_count];

void *qs(void *ptr);
void swap(int *a,int *b);
struct Params //передаю значения в метод QS
{
	int* s_arr;
	int first;
	int last;
};

int main(void)
{
	int s,index,i,length=50; //счетчик и длинна массива
	int a[length];//массив

	srand(time(NULL ));
	//создал массив
	for(i=0;i<=length;i++) a[i]=rand()%100;
	for(i=0;i<=length;i++) printf("%d ", a[i]);printf("\n");


	s=length/th_count; //длина части массива обрабатываемого 1  потоком
	for(index=0; index<th_count;index++)
	{
		struct Params p = { a, index*s, index*s+s};
		if (index == (th_count - 1))
			p.last=length;
		//потоки создаем-с
		pthread_create(&(array_of_threads[index]), NULL, qs, (void*) &p);
	}
	//у нас созданы потоки, которые параллельно сортируют свои части массива. Теперь главный ждет пока они все завершат работу:
	for(index=0; index<th_count;index++)
	{
		pthread_join(array_of_threads[index], NULL);
	}
	//досортировываем
	struct Params p = { a, 0, length};
	qs(&p);
	//вывод массива
	printf("\n");
	for(i=0;i<=length;i++) printf("%d ", a[i]);//Вывод масссива
	return 0;
}

void *qs(void *ptr) //сортировка массива
{
	struct Params *p;
	p = (struct Params *) ptr;//читаем переданные параметры
	int i = p->first, j = p->last, x = p->s_arr[(p->first + p->last) / 2];
	do
	{
	while (p->s_arr[i] < x) i++;
	while (p->s_arr[j] > x) j--;
		if(i <= j)
		{
			if (i < j) swap(&p->s_arr[i], &p->s_arr[j]);//раскидываем в разные стороны от осевого элемента
			i++;
			j--;
		}
	}
	while (i <= j);

	//повторяем рекурсивно
	if (i < p->last)
	{
		struct Params p1 = { p->s_arr, i, p->last};
		qs(&p1);
	}
	if (p->first < j)
	{
		struct Params p1 = { p->s_arr, p->first,j};
		qs(&p1);
	}
	return 0;
}

void swap(int *a,int *b)
{
	int t;
	t = *a, *a = *b, *b = t;
}
