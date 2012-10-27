#include <stdio.h>
#include <pthread.h>
#include <string>
#include <vector>

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
    std::vector<std::string> tmp = CE.Execute(std::string(buf));
    std::string result = "";
    for (int i = 0; i < tmp.size(); i++)
        result += tmp[i];
    //printf("%d:%d exec: %s\n", pthread_self(), fd, result.c_str());
    write(fd, result.c_str(), strlen(result.c_str()));
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
