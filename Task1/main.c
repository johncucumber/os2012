#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h> 
#include <signal.h>
#include <semaphore.h>
#include <assert.h>
#include "queue.h"

#define MAX 16 // Колличество элементов
#define THC 4
sem_t sem;

void *Sort();

int main(int argc, char **argv) {

	pthread_t thread[THC];

	int a[MAX];
	int i;

	sem_init(&sem, 0, 0);

	srand(1);

	for (i = 0; i < MAX; i++) {

	}

	for (i = 0; i < MAX; i++) {
		printf("%d ", a[i] = rand() % 10);
	}
	printf("\n");

	for (i = 0; i < THC; i++) {
		pthread_create(&thread[i], NULL, Sort, NULL );
	}

	for (i = 0; i < THC; i++) {
		struct Node param;
		param.size = 4;
		param.first = a + 4 * i;
		push(&param);
		sem_post(&sem);

	}
	for (i = 0; i < MAX; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	
	for (i = 0; i < 2; i++) {
		struct Node param;
		param.size = 8;
		param.first = a + 8 * i;
		push(&param);
		sem_post(&sem);

	}
	for (i = 0; i < MAX; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	for (i = 0; i < 1; i++) {
		struct Node param;
		param.size = 16;
		param.first = a;
		push(&param);
		sem_post(&sem);
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
		struct Node *param;
		param = pop(); 
		int *a = param->first;
		int size = param->size;
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
	return 0;
}


