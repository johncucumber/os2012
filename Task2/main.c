#include <stdio.h>
#include <pthread.h> 
#include <semaphore.h>

int THC; 

int main(int argc, char **argv) {
	
	/* Задаем кооличество потоков*/
	if (argc == 2) {
		THC = atoi(argv[1]);
	} else
	printf("Please set thread count\n");
	return 0;
}