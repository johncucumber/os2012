#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>



int run = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_t thread1;
pthread_t thread2;

void *run_th1(void *ptr);
void *run_th2(void *ptr); 
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
	int i,length=50; //счетчик и длинна массива
	int a[length];//массив
		
	//создал массив
	for(i=0;i<=length;i++) a[i]=rand()%100;
	for(i=0;i<=length;i++) printf("%d ", a[i]);printf("\n");	
	//потоки создались
	struct Params p = { &a, 0, length};
	pthread_create(&thread1, NULL, run_th1, &p);
    pthread_create(&thread2, NULL, run_th2, &p);

	getchar();
	//вывод массива
	printf("\n");
	for(i=0;i<=length;i++) printf("%d ", a[i]);//Вывод масссива
	return 0;
}

 //thread A  Сортируем 2 куска
void *run_th1(void *ptr)
{		
	struct Params *p;
	p = (struct Params *) ptr;//читаем переданные параметры
	int length=p->last;
	struct Params p1 = { p->s_arr, 0, (length-1)/2 };
	qs(&p1);
	p1.first=p1.last;p1.last=(length)-1;
    qs(&p1);    
	
    // Разблокируем второй поток, который сортирует эти части вместе 
    pthread_mutex_lock(&lock);
    run = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
}


// thread B  
void *run_th2(void *ptr){	
	//проверка на возможность запуска
	pthread_mutex_lock(&lock);
    while (!run)
        pthread_cond_wait(&cond, &lock);
		run = 0;
    pthread_mutex_unlock(&lock);

	     
    struct Params *p;
	p = (struct Params *) ptr;//читаем переданные параметры
	int length=p->last;
	struct Params p1 = { p->s_arr, 0, length-1};
	qs(&p1);
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
		struct Params p1 = { p->s_arr, i, p->last };
        qs(&p1);
    }
    if (p->first < j)
    {
		struct Params p1 = { p->s_arr, p->first,j };
        qs(&p1);
	}
}

void swap(int *a,int *b)
{
    int t;    
    t = *a, *a = *b, *b = t;
}
