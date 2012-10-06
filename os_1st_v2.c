#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#define th_count 6
pthread_t array_of_threads [th_count];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 
void *qs(void *ptr);
void swap(int *a,int *b);
struct Params //передаю значения в метод QS
{
	int* s_arr;
	int first;
	int last;
	int n; //номер потока
};
 
int main(void)
{	
	int length=19;
	int a[length];
	int s,i,index;
	
	//создал массив
	for(i=0;i<=length;i++) a[i]=rand()%100;
	for(i=0;i<=length;i++) printf("%d ", a[i]);printf("\n");	
	
	for(i=1;i<=th_count;i++)
	{
		s=length/th_count/i; //длина части массива обрабатываемого 1  потоком
		for(index=0; index<th_count/i;index++)
		{
			struct Params p = { &a, index*s, index*s+s*i, index};
			if (index == (th_count - i))
				p.last=length;
			//потоки создаем-с
			pthread_create(&(array_of_threads[index]), NULL, qs, (void*) &p); 
			pthread_join(array_of_threads[index], NULL);			
		}		
	}
	//getchar();
	printf("\n");
	for(i=0;i<=length;i++) printf("%d ", a[i]);//Вывод масссива
	return 0;
}

void *qs(void *ptr) 
{
	struct Params *p;
	p = (struct Params *) ptr;//читаем переданные параметры
    int i = p->first, j = p->last, x = p->s_arr[(p->first + p->last) / 2];
      
    do {
        while (p->s_arr[i] < x) i++;
        while (p->s_arr[j] > x) j--;
 
        if(i <= j) {
            if (i < j) 
            {
				pthread_mutex_lock(&mutex);
				swap(&p->s_arr[i], &p->s_arr[j]);//раскидываем в разные стороны от осевого элемента
				pthread_mutex_unlock(&mutex);
            }
            i++;
            j--;
        }
    } while (i <= j);
	//повторяем рекурсивно
    if (i < p->last)
    {
		struct Params p1 = { p->s_arr, i, p->last,p->n };
        qs(&p1);
    }
    if (p->first < j)
    {
		struct Params p1 = { p->s_arr, p->first,j,p->n };
        qs(&p1);
	}
}

void swap(int *a,int *b)
{
    int t;    
    t = *a, *a = *b, *b = t;
}
