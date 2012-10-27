#include <stdio.h>
#include <pthread.h>
#include <string>

#include "comandexecuter.h"
#include "taskmanager.h"
#include "epollreciver.h"

void* ServeClient(void *d)
{    
    ComandExecuter CE;
    int fd = (long)d;
    printf("%d:%d receiv fd\n", pthread_self(), fd);
    char buf[512];
    read(fd, buf, sizeof buf);
    printf("%d:%d read %s\n", pthread_self(), fd, buf);
    write(fd, "ok", 3);
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("<usage> <port> <number_of_thread>\n");
        return 0;
    }
    ThreadPool tp(atoi(argv[2]), ServeClient);
    EpollReceiver er;
    er.Start(argv[1], tp);

    return 0;
}
