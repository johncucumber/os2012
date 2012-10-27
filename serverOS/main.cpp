#include <stdio.h>
#include <pthread.h>
#include <string>

#include "comandexecuter.h"
#include "taskmanager.h"
#include "epollreciver.h"
void* f(void *d)
{    
    printf("Hi! from %d: %d\n", pthread_self(), d);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("<usage> <port> <number_of_thread>\n");
        return 0;
    }
    ThreadPool tp(atoi(argv[2]), f);
    EpollReceiver er;
    er.Start(argv[1], tp);

    return 0;
}
