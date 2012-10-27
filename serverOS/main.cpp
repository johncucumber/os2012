#include <stdio.h>
#include <pthread.h>
#include <string>

#include "comandexecuter.h"
#include "taskmanager.h"
#include "task.h"

void* f(void *d)
{
    std::string buf = *(std::string *)d;
    printf("Hi! from %d: %s\n", pthread_self(), buf.c_str());
}

int main(int argc, char *argv[])
{
    printf("Hell\n");
    ThreadPool tp(10, f);
    for (int i = 0; i < 20; i++)
    {
        std::string *tmp = new std::string("Hi!");
        Task t(false, tmp);
        tp.AddTask(t);
    }
    //ComandExecuter ce;
    //ce.PrintVec(ce.Execute(argv[1]));
    for (int i = 0; i < 20000; i++)
    {

    }
    return 0;
}
