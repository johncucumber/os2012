#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h> 
#include <signal.h>
#include <semaphore.h>
#include <assert.h>
#include <math.h>
#include "queue.h"

#define MAX 16// Колличество элементов
#define THC 4
sem_t sem;
sem_t endsem;

void *Sort();

int main(int argc, char **argv) {

	int a[MAX];
	int i;

	sem_init(&sem, 0, 0);
	sem_init(&endsem, 0, 0);

	srand(1);

	for (i = 0; i < MAX; i++) {

	}

	for (i = 0; i < MAX; i++) {
		printf("%d ", a[i] = rand() % 10);
	}
	printf("\n");

	for (i = 0; i < THC; i++) {
		pthread_t thread;
		pthread_create(&thread, NULL, Sort, NULL );
	}

	int j, size;
	int level = log(2 * THC) / log(2);
	size = MAX / THC;
	int t_c = THC;
	for (j = 0; j < level; j++) {
		for (i = 0; i < t_c; i++) {
			struct Node param;
			param.size = size;
			param.first = a + size * i;
			push(&param);
		}
		for (i = 0; i < t_c; i++) {
			sem_post(&sem);
			sleep(1);
		}
		t_c /= 2;
		size *= 2;
	}

	sem_wait(&endsem);
	for (i = 0; i < MAX; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	return 0;
}

void *Sort() {
	while (1) {
		struct Node *param;
		sem_wait(&sem);
		param = pop();
		int *a = param->first;
		int size = param->size;
		printf("size %d :  ", size);
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

		}

		else {
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
		int i;
		for (i = 0; i < size; i++) 
		printf("%d ", a[i]);
	    printf("\n");
		if (size == MAX) {
			sem_post(&endsem);
			return NULL ;
		}
	}
	return 0;
}
