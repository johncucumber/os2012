#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define n 100000000
int s_arr[n];

void swap(int *a,int *b);
void qs(int first, int last);
int main(void)
{
	int i;	
	srand(time(NULL));
	for(i=0;i<n;i++) s_arr[i]=rand()%100;
	qs(0,n-1);
	return 0;
}

void qs(int first,int last)
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
}

void swap(int *a,int *b)
{
	int t;
	t = *a, *a = *b, *b = t;
}
