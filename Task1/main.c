#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 
#include <signal.h>
#include <semaphore.h>
#include <assert.h>
#include <math.h>
#include "queue.h"

sem_t sem;
sem_t endsem;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
int THC = 4;
int MAX = 32;
struct Queue *q;
int * start;

void swap(int *a,int *b);
void *Sort();
void bubble(int* a, int size);
void qs(int* s_arr, int first, int last);

int main(int argc, char **argv) {
	
	/* Задаем кооличество потоков и элементов*/
	if (argc == 3) {
		THC = atoi(argv[1]);
	    MAX = atoi(argv[2]);
	} else
	printf("use args THREAD_COUNT  ELEMENTS_COUNT\n");
	
	sem_init(&sem, 0, 0);
	sem_init(&endsem, 0, 0);
	srand(1);
	
	int a[MAX];
	start = a;
	
	int i;
	
	
	for (i = 0; i < MAX; i++) {
		a[i] = rand() % 10;
	}
	
	
	/* Создаем потоки */
	for (i = 0; i < THC; i++) {
		pthread_t thread;
		pthread_create(&thread, NULL, Sort, NULL );
	}
	
	/* Создаем очередь заданий и выполняем*/
	
	q = new_queue();
	
	int j, size;
	int level = (int)(ceil((log(THC)/log(2)))+1);
	size = MAX / THC;
	int t_c = THC;
	for (j = 0; j < level; j++) {
		for (i = 0; i < t_c; i++) {
			push(q, a + size * i,size);
			sem_post(&sem);
		}
		for (i = 0; i < t_c; i++) {
			
			sem_wait(&endsem);
		}
		t_c /= 2;
		size *= 2;
	}
	for (i = 0; i < MAX; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	
	return 0;
}

void *Sort() {
	while (1) {
		sem_wait(&sem);
		pthread_mutex_lock(&mymutex);
		struct Node *param;
		param = pop(q);
		pthread_mutex_unlock(&mymutex);
		int *a = param->first;
		int size = param->size;
		printf("id %d; first %d; second %d\n", pthread_self(),a-start,a-start+size);
		fflush(stdout);
		if (size == MAX / THC) {
			
			int i, j;
			int tmp;
			for (i = 0; i < size; i++) {
				for (j = size - 1; j > i; j--) {
					if (*(a + j - 1) > *(a + j)) {
						tmp = *(a + j - 1);
						*(a + j - 1) = *(a + j);
						*(a + j) = tmp;
					}
				}
			}

		} else {
			int i = 0;
			int j = size / 2;
			int k = 0;

			int b[size];

			while (i < size / 2 && j < size) {
				if (*(a + i) < *(a + j)) {
					b[k] = *(a + i);
					i++;
				} else {
					b[k] = *(a + j);
					j++;
				}
				k++;
			}

			while (i < size / 2) {
				b[k] = a[i];
				k++;
				i++;
			}

			while (j < size) {
				b[k] = a[j];
				k++;
				j++;
			}
			for (i = 0; i < size; i++) {
				*(a + i) = *(b + i);
			}
		}
		/*int i;
		for (i = 0; i < size; i++)
			printf("%d ", a[i]);
		printf("\n");*/
		sem_post(&endsem);
	}
	return 0;
}
