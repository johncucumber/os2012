#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
 
void *qs(void *ptr);
struct Params //передаю значения в метод QS
{
	int* s_arr;
	int first;
	int last;
};
 
int main(void)
{	
	pthread_t thread;
	int i;
	
	int a[10]={25,1,12,8,9,5,44,10,2,19};//создал массив
	for(i=0;i<=9;i++) printf("%d ", a[i]);printf("\n");	
	
	struct Params p = { &a, 0, 9 };
	pthread_create(&thread, NULL, qs, (void*) &p);//создал поток с методом QS
	pthread_join(thread, NULL );
	
//	qs(&a,0,9);
	for(i=0;i<=9;i++) printf("%d ", a[i]);//Вывод масссива
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
            if (i < j) swap(&p->s_arr[i], &p->s_arr[j]);//раскидываем в разные стороны от осевого элемента
            i++;
            j--;
        }
    } while (i <= j);
 
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
