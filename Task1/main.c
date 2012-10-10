#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h> 
#include <signal.h>
#include <semaphore.h>
#include <assert.h>

#define MAX 16 // Колличество элементов
#define THC 4  // Колличество потоков
sem_t sp1;
sem_t sp2;

pthread_t thread[THC];
struct Params {
	int size;
	int *first;
	int number;
	int pr;
};
struct Params p[THC];

void *Sort(void *ptr);

int main(void) {

	int a[MAX];
	int size;

	srand(time(NULL ));

	int i = 0;
	for (i = 0; i < MAX; i++) {
		//a[i] = MAX - i;
		a[i] = rand() % (10);
	}

	sem_init(&sp1, 0, 0);
	sem_init(&sp2, 0, 0);

	for (i = 0; i < MAX; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	size = (MAX / THC);

	for (i = 0; i < THC; i++) {
		//if (i == (THC - 1))
		//	size = MAX - (THC - 1) * size; // Последний поток берет остатки 

		p[i].size = size;
		p[i].first = a + size * i;
		p[i].number = i + 1;
		p[i].pr = 1;
		//Создание потоков
		pthread_create(&thread[i], NULL, Sort, (void*) &p[i]);
	}
	pthread_join(thread[0], NULL );

	for (i = 0; i < MAX; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	return 0;
}

void *Sort(void *ptr) {

	struct Params *p;
	p = (struct Params *) ptr;

	int *a = p->first;
	int size = p->size;
	int number = p->number;
	int pr = p->pr;
	int exit = 1;

	while (exit) {

		if (pr == 1) {
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


		if (size == MAX) {
			return 0;
		}

		if (pr == 1) {
			if (number == 1) {
				sem_wait(&sp1);
				size *= 2;
			}
			if (number == 3) {
				sem_wait(&sp2);
				size *= 2;
			}
			if (number == 2) {
				sem_post(&sp1);
				return 0;
			}
			if (number == 4) {
				sem_post(&sp2);
				return 0;
			}
		}

		if (pr == 2) {
			if (number == 1) {
				sem_wait(&sp1);
				size *= 2;
			} else {
				sem_post(&sp1);
				return 0;
			}
		}

		pr++;

	}

	return NULL ;
}
