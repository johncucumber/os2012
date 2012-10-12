#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h> 
#include <signal.h>
#include <semaphore.h>
#include <assert.h>

#define MAX 16 // Колличество элементов
sem_t *sp;
int th_count;

struct Params {
	int size;
	int *first;
	int number;
	int pr;
};

void *Sort(void *ptr);

int main(int argc, char **argv) {

	if (argc != 2) {
		printf("use arg\n");
		return 0;
	}
	int THC = th_count = atoi(argv[1]);
	sem_t s[THC / 2];
	sp = s;
	int i;
	for (i = 0; i < THC / 2; i++) {
		sem_init(sp + i, 0, 0);
	}

	pthread_t thread[THC];
	struct Params p[THC];

	int a[MAX];
	int size;

	srand(1);

	for (i = 0; i < MAX; i++) {
		a[i] = rand() % 10;
	}

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
	int semnum = 0;

	while (1) {

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

		int i;
		printf("thread %d Pr %d  :", number, pr);
		for (i = 0; i < size; i++) {
			printf("%d ", a[i]);
		}
		printf("\n");

		if (size == MAX) {
			return 0;
		}

		if (number % 2) {
			semnum = number / 2 ;
			sem_wait(sp + semnum);
			size *= 2;
			
			if (number != 1)
			sem_post(sp + semnum);
			
			for (i = semnum; i < th_count/2-semnum; i++) {
			sem_wait(sp + i);
			sem_post(sp + i);
		    }
			
			
		} else {
			semnum = (number / 2) - 1;
			sem_post(sp + semnum);
			return 0;
		}
		
		number = number == 1 ? 1 : (number+1) / 2;

		pr++;
		th_count/=2;

	}

	return 0;
}
