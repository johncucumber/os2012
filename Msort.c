#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

const ArrLong=256;
const ThrCount=16;

struct Params {
	int size;
	int *array;
	int first;
	int number;
	int retry;	
	int musthave;
	int nya;
	pthread_t *thread;
};

void Merge(int *Mas, int left, int right, int medium)
{
    int i;
    int j = left;
    int k = medium + 1;
    int count = right - left + 1;
 
    if (count <= 1) return;
 
    int TmpMas[count];
 
    for (i = 0; i < count; i++)
    {
        if (j <= medium && k <= right)
        {
            if (Mas[j] < Mas[k])
                TmpMas[i] = Mas[j++];
            else
                TmpMas[i] = Mas[k++];
        }
        else
        {
            if (j <= medium)
                TmpMas[i] = Mas[j++];
            else
                TmpMas[i] = Mas[k++];
        }
    }
 
    j = 0;
    for (i = left; i <= right; i++)
    {
        Mas[i] = TmpMas[j++];
    }
}
 
void MergeSort(int *a, int l, int r)
{
    int m;
 
    if(l >= r) return;
 
    m = (l + r) / 2;
 
    MergeSort(a, l, m);
    MergeSort(a, m + 1, r);
    Merge(a, l, r, m);
}


void * thread_func(void *arg)
{ 
	int i,tmp;		
	struct Params *p;
	p = (struct Params *) arg;

	int size = p->size;
	int *array = p->array;
	int first = p->first;
	int number = p->number;	
	int retry = p->retry;	
	int musthave = p->musthave;
	int nya = p->nya;
	pthread_t *thread = p->thread;

	if (retry==1){
		MergeSort(array,first,first+size-1);
		retry++;
	} 
	for (i=2;i<=nya;i++){
		int tmp;
		if (retry==i){
			musthave=0;
			tmp = 0;
			while (tmp<ThrCount)
			{
				if (number==tmp){
					int temp = (int)(pow(2,retry-2));
					pthread_join(thread[number+temp],NULL);
					Merge(array, first, first+(pow(2,retry-1))*size-1, first+(pow(2,(retry-2))*size)-1);
					musthave=1;
				}
				tmp+=pow(2,retry-1);
			}
			if (musthave==0)
			{
				return;
			}
		}
		retry++;
	}
}

int main(int argc, char * argv[])
{
	double DThrCount = (double)ThrCount;
	int nya = (int)(ceil((log(DThrCount)/log(2))))+1;

	int arr[ArrLong],i,size,result;
	pthread_t thread[ThrCount];
	struct Params p[ThrCount];

	srand(1);

	size = (ArrLong / ThrCount);

	for (i = 0; i < ArrLong; i++) {
		arr[i] = rand() % 100;
	}

	for (i = 0; i < ArrLong; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");	

	for (i = 0; i < ThrCount; i++) {
		p[i].size = size;
		p[i].array = arr;
		p[i].first = size * i;
		p[i].number = i;
		p[i].retry = 1;
		p[i].musthave = 0;
		p[i].nya = nya;
		p[i].thread = thread;
		result=pthread_create(&thread[i], NULL, thread_func, (void*) &p[i]);
		if (result != 0) {
			perror("Creating the thread");			
		}
	}
	pthread_join(thread[0], NULL );	

	for (i = 0; i < ArrLong; i++) {
	printf("%d ", arr[i]);
	}
	printf("\n");
	return 0;
}
