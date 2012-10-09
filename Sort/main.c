#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>

#define MAX 100000 // Колличество элементов
#define THC 4  // Колличество потоков
pthread_mutex_t mutex;

void *bubbleSort(void *ptr);

struct Params {
	int size;
	int *first;
	int *b;
};

int main(void) {
	pthread_t thread;
	int a[MAX];
	int b[MAX];
	int size;

	srand(time(NULL ));

	// Заполенение массивов
	int i = 0;
	for (i = 0; i < MAX; i++) {
		a[i] = rand() % MAX;
		b[i] = INT_MAX;
	}

	size = (MAX / THC);

	pthread_mutex_init(&mutex, NULL );

	for (i = 0; i < THC; i++) {
		if (i == (THC - 1))
			size = MAX - (THC - 1) * size; // Последний поток берет остатки
		struct Params p = { size, a + size * i, b };

		//Создание потоков
		pthread_create(&thread, NULL, bubbleSort, (void*) &p);
		pthread_join(thread, NULL );
	}

	// Проверка
	for (i = 1; i < MAX; i++) {
		if (b[i - 1] > b[i])
			break;
	}
	if (i < MAX)
		printf("Ошибка: %d > %d", b[i - 1], b[i]);
	else
		printf("Нет ошибки");
	return 0;
}

/* Пузырьковая сортировка */
void *bubbleSort(void *ptr) {
	struct Params *p;
	p = (struct Params *) ptr;

	int *a = p->first;
	int size = p->size;
	int *b = p->b;

	int i, j;
	int x;

	// Сортировка
	for (i = 0; i < size; i++) {
		for (j = size - 1; j > i; j--) {
			if (*(a + j - 1) > *(a + j)) {
				x = *(a + j - 1);
				*(a + j - 1) = *(a + j);
				*(a + j) = x;
			}
		}
	}

	// Запихивание в массив
	pthread_mutex_lock(&mutex);
	for (i = 0; i < size; i++) {
		int k = 0;
		while (*(a + i) > *(b + k)) {
			k++;
		}
		int j;
		for (j = MAX; j > k; j--)
			*(b + j) = *(b + j - 1);
		*(b + k) = *(a + i);
	}
	pthread_mutex_unlock(&mutex);

	return 0;
}
