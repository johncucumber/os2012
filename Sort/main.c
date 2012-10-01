#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 40 // Колличество элементов
#define THC 4  // Колличество потоков
void *bubbleSort(void *ptr);

struct Params {
	int size;
	int *first;
};

int main(void) {
	pthread_t thread;
	int a[MAX];
	int size;

	srand(time(NULL ));

	// Заполенение массивов
	int i = 0;
	for (i = 0; i < MAX; i++) {
		a[i] = rand() % MAX;
	}

	size = (MAX / THC);
	//printf("%d \n", a);
	for (i = 0; i < THC; i++) {
		
		struct Params p = {size,a + size * i};
		
		//Создание потоков
		pthread_create(&thread, NULL, bubbleSort, (void*) &p);
		//printf("%d \n",  &p);
		pthread_join(thread, NULL );
	}

	// Вывод результата
	i = 0;
	for (i = 0; i < MAX; i++) {
		printf("%d \n", a[i]);
	}
	return 0;
}

/* Пузырьковая сортировка */
void *bubbleSort(void *ptr) {
	struct Params *p;
	p = (struct Params *) ptr;

	int *a = p->first;
	int size = p->size;
	
	//printf("%d \n", (p->first));
	//printf("%d \n", size);

	int i, j;
	int x;
	for (i = 0; i < size; i++) {
		for (j = size - 1; j > i; j--) {
			if (*(a + j - 1) > *(a + j)) {
				x = *(a + j - 1);
				*(a + j - 1) = *(a + j);
				*(a + j) = x;
			}
		}
	}
	return 0;
}
