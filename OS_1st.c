#include <stdio.h>
#include <math.h>
#include <unistd.h>
 
int main(void)
{	int i;
	int a[10]={25,1,12,8,9,5,44,10,12,19};
	for(i=0;i<=9;i++) printf("%d ", a[i]);printf("\n");
	
	qs(&a,0,9);
	for(i=0;i<=9;i++) printf("%d ", a[i]);
	return 0;
}

void qs(int* s_arr, int first, int last)
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
        qs(s_arr, i, last);
    if (first < j)
        qs(s_arr, first,j);
}

void swap(int *a,int *b)
{
    int t;    
    t = *a, *a = *b, *b = t;
}
