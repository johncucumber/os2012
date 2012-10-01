#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 30
void *print_message_function(void *ptr);
void *bubbleSort(void *ptr);

struct Params {
	int size;
	int * first;
};

int main(void) {
	pthread_t thread;
	int a[MAX];

	srand(time(NULL ));

	// Заполенение массивов
	int i = 0;
	for (i = 0; i < MAX; i++) {
		a[i] = rand() % MAX;
	}

	struct Params *p1;
	
	p1->size = (MAX / 2);
	printf("s %d\n",p1->size);

	for (i = 0; i < 2; i++) {
		p1->first = a + p1->size * i+i;
		//Создание потоков
		pthread_create(&thread, NULL, bubbleSort, (void*) p1);
		pthread_join(thread, NULL );
	}
	
	// Вывод результата
	i = 0;
	for (i = 0; i < MAX; i++) {
		printf("%d \n", a[i]);
	}
	return 0;
}

/* Тестовая функция для вывода массива */
void *print_message_function(void *ptr) {
	int *a;
	a = (int *) ptr;

	int i = 0;
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
