
/*#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include<pthread.h>
#include<stdbool.h>
#include <string.h>
#include "comandexecuter.h"
#include <string>

#define NAME_LENGTH 32
#define MESSAGE_LENGTH 1024

pthread_mutex_t mutex;
pthread_mutex_t pipe_mutex;

struct Message
{
    char sender[NAME_LENGTH];
    char text[MESSAGE_LENGTH];
    char text1[32000];
    char text2[30000];
};

struct Task
{
    int clientfd;
    struct List *clients;
    struct Message message;
    int message_size;
    bool finish;
};

struct ServerData
{
    int threads_count;
    bool *finish;
    bool *inf_mode;
};

struct ListElement
{
    int value;
    struct ListElement *next;
};

struct List
{
    int count;
    struct ListElement *first;
    struct ListElement *last;
};

bool ListIsEmpty(struct List *list)
{
    return((list->count) < 1);
}

void AddToList(struct List *list, int value)
{
    pthread_mutex_lock(&mutex);
    struct ListElement *element = (struct ListElement *)malloc(sizeof(struct ListElement));
    element->value = value;
    element->next = NULL;
    if(ListIsEmpty(list))
    {
        list->first = element;
        list->last = element;
    }
    else
    {
        list->last->next = element;
        list->last = element;
    }
    (list->count)++;
    pthread_mutex_unlock(&mutex);
}

void RemoveFromList(struct List *list, int value)
{
    pthread_mutex_lock(&mutex);
    if(!ListIsEmpty(list))
    {
        struct ListElement *element = list->first;
        struct ListElement *found_element = NULL;
        if(element->value == value)
        {
            found_element = element;
            list->first = element->next;
            (list->count)--;
            free(found_element);
        }
        else
            while(element->next != NULL)
            {
                if (element->next->value == value)
                {
                    found_element = element->next;
                    if(found_element == list->last)
                        list->last = element;
                    element->next = found_element->next;
                    (list->count)--;
                    free(found_element);
                    break;
                }
                else
                    element = element->next;
            }
    }
    pthread_mutex_unlock(&mutex);
}

void ClearList(struct List *list)
{
    pthread_mutex_lock(&mutex);
    if(!ListIsEmpty(list))
    {
        struct ListElement *element = list->first;
        struct ListElement *current_element = NULL;
        while(element != NULL)
        {
            current_element = element;
            element = element->next;
            (list->count)--;
            free(current_element);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void InitializeList(struct List *list)
{
    list->count = 0;
    list->first = NULL;
    list->last = NULL;
}


void* Broadcast(void *data)
{
    int pipe_read_fd = *((int *)data);
    struct Task task;
    while(1)
    {
        pthread_mutex_lock(&pipe_mutex);
        read(pipe_read_fd, &task, sizeof(task));
        pthread_mutex_unlock(&pipe_mutex);
        //printf("read task\n");
        if (task.finish)
        {
            printf("Thread finished\n");
            break;
        }
        ComandExecuter CE;
        //pthread_mutex_lock(&mutex);
        struct ListElement *element = task.clients->first;
        std::vector<std::string> tmp = CE.Execute(std::string(task.message.text));
        std::string result = "";
        for (int i = 0; i < tmp.size(); i++)
            result += tmp[i];
        write(task.clientfd, result.c_str(), strlen(result.c_str()));
        /*while(element != NULL)//отправляем сообщение всем подключенным клиентам, кроме отправителя
        {
            if((element->value) != task.clientfd)
                send(element->value, &(task.message), task.message_size, 0);
            element = element->next;
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* Server(void *data)
{
    printf("Server started\n");
    printf("Commands: stop, inf\n");
    int listenfd;
    struct sockaddr_in addr;
    struct ServerData server_data = *((struct ServerData *)data);
    int threads_count = server_data.threads_count;

    const int MAX_EPOLL_EVENTS = 100;
    const int BACK_LOG = 100;

    listenfd = socket(AF_INET, SOCK_STREAM, 0); //создание internet-сокета (потокового) (это сокет для приема запросов на соединение от клиентов)
    if(listenfd < 0)
    {
        perror("Socket creation error\n");
        exit(1);
    }

    fcntl(listenfd, F_SETFL, O_NONBLOCK); //назначение сокета неблокирующим

    addr.sin_family = AF_INET;
    addr.sin_port = htons(10023); //номер порта сокета
    addr.sin_addr.s_addr = INADDR_ANY; //ip-адрес сетевого интерфейса, через который совершается соединение (здесь - любой)
    if(bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) //связывание сокета с адресом
    {
        perror("Binding error");
        exit(2);
    }

    listen(listenfd, BACK_LOG); //перевод сервера в режим ожидания запросов

    struct List list;//список дескрипторов сокетов подключенных клиентов
    InitializeList(&list);



    // Создание дескриптора epoll
    int efd = epoll_create(MAX_EPOLL_EVENTS);

    // Добавляем дескриптор слушающего сокета в массив ожидания событий
    struct epoll_event listenev;
    listenev.events = EPOLLIN /*есть данные для чтения| EPOLLPRI/*есть срочные данные для чтения*| EPOLLET/*Edge-Triggered/;//флаги отслеживаемых событий
    listenev.data.fd = listenfd;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &listenev) < 0) //теперь epoll следит за наступлением указанных событий для сокета listenfd
    {
        perror("Epoll fd add");
        return 0;
    }

    socklen_t client;

    struct epoll_event events[MAX_EPOLL_EVENTS]; //структуры epoll_event для всех наступивших событий
    struct epoll_event connev;
    struct sockaddr_in cliaddr; //структура для сохранения в ней адреса подключаемого клиента

    int events_count = 1; //число сокетов, для которых отслеживаются события

    int pfd[2];

    if (pipe(pfd) == -1)// канал для заданий
    {
        perror("Pipe creation error\n");
        exit(EXIT_FAILURE);
    }

    int pipe_read_fd = pfd[0];
    int pipe_write_fd = pfd[1];

    int i;

    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * threads_count);
    for(i = 0; i < threads_count; i++)
    {
        if(*(server_data.inf_mode))
            printf("Creating thread %d\n", i);

        int error = pthread_create(&(threads[i]), NULL, Broadcast, (void*)(&pipe_read_fd));

        if(error)
            printf("Thread creation error!\n");
    }

    while(!*(server_data.finish))
    {
        // Блокирование до готовности одного или нескольких дескрипторов
        int nfds = epoll_wait(efd, events, MAX_EPOLL_EVENTS, 1000);//когда наступает хотя бы одно событие, функция возвращает число наступивших событий, а их структуры записывает в events
        if(nfds < 1)
            continue;

        //Вывод списка клиентов
        if(*(server_data.inf_mode))
        {
            pthread_mutex_lock(&mutex);
            printf("========================Connected clients========================\n");
            struct ListElement *element = list.first;
            while(element != NULL)
            {
                printf("client %d\n", element->value);
                element = element->next;
            }
            pthread_mutex_unlock(&mutex);
        }

        int n;
        for (n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == listenfd)// Готов слушающий дескриптор
            {
                client = sizeof(cliaddr);
                int connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &client); //принять запрос на соединение от клиента
                if (connfd < 0)
                {
                    perror("accept");
                    continue;
                }


                if (events_count == MAX_EPOLL_EVENTS-1)// Недостаточно места в массиве ожидания, закрываем соединение
                {
                    printf("Event array is full\n");
                    close(connfd);
                    continue;
                }

                // Добавление клиентского дескриптора в массив ожидания
                fcntl(connfd, F_SETFL, O_NONBLOCK); //назначение сокета неблокирующим
                connev.data.fd = connfd;
                connev.events = EPOLLIN /*есть данные для чтения*//*| EPOLLOUT /*готов для записи| EPOLLET/*edge-triggered | EPOLLRDHUP/*сокет клиента закрыл соединение/;
                if (!epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &connev) < 0) //добавляем, теперь epoll отслеживает события и для этого сокета
                {
                    perror("Epoll fd add");
                    close(connfd);
                    continue;
                }
                AddToList(&list, connfd);// Добавляем сокет в список клиентов

                ++events_count;
            }
            // Готов клиентский дескриптор
            else
            {
                // Выполням работу с дескриптором
                int fd = events[n].data.fd;//получаем дескриптор из структур epoll_event массива events

                // новое задание
                if (events[n].events & EPOLLIN)// Поступили данные от клиента, читаем их
                {
                    char buf[sizeof(struct Message)];
                    int bytes_read = 0;
                    int bytes_left = sizeof(struct Message);

                    /*while(bytes_left > 0 && !*(server_data.finish))
                    {
                        int bytes = recv(fd, buf + bytes_read, bytes_left, 0);
                        if(bytes > 0)
                        {
                            bytes_read += bytes;
                            bytes_left -= bytes;
                            if(*(server_data.inf_mode))
                                printf("bytes_read: %d bytes_left: %d\n", bytes_read, bytes_left);
                        }

                        if(bytes == 0) // Соединение разорвано, удаляем сокет из epoll и списка
                        {
                            epoll_ctl(efd, EPOLL_CTL_DEL, fd, &connev);
                            --events_count;
                            close(fd);
                            RemoveFromList(&list, fd);
                            break;
                        }
                    }
                    if (bytes_left > 0)
                    {
                        continue;
                    }


                    if(*(server_data.inf_mode))
                        printf("Read %d of %d bytes.\n", bytes_read, sizeof(struct Message));
                    //заполняем задание
                    struct Task inf;
                    inf.finish = false;
                    inf.clientfd = fd;
                    inf.message = *((struct Message *)buf);
                    inf.message_size = bytes_read;
                    inf.clients = &list;
                    write(pipe_write_fd, &inf, sizeof(inf));//записываем задание в канал
                }

                /*if (events[n].events & EPOLLOUT)
                {
                    do_write(fd);
                }

                if (events[n].events & EPOLLRDHUP)//Клиент закрыл соединение на своей стороне, удаляем сокет
                {
                    epoll_ctl(efd, EPOLL_CTL_DEL, fd, &connev);
                    --events_count;
                    close(fd);
                    RemoveFromList(&list, fd);
                    continue;
                }
            }
        }
    }
    printf("Creating finish tasks\n");

    for(i=0; i<threads_count; i++)
    {
        struct Task task;
        task.finish = true;
        //pthread_mutex_lock(&mutex);
        write(pipe_write_fd, &task, sizeof(task));//записываем задание в канал
        //pthread_mutex_unlock(&mutex);
    }
    printf("Finish tasks created\n");


    void *status;

    for(i=0; i<threads_count; i++)//Waiting for threads
    {
        pthread_join(threads[i], &status);
    }
    printf("All threads joined\n");

    pthread_mutex_lock(&mutex);
    struct ListElement *element = list.first;
    while(element != NULL)
    {
        int fd = element->value;
        epoll_ctl(efd, EPOLL_CTL_DEL, fd, &connev);
        --events_count;
        close(fd);
        element = element->next;
    }
    pthread_mutex_unlock(&mutex);
    close(listenfd);
    printf("All sockets closed\n");

    close(pipe_read_fd);
    close(pipe_write_fd);

    free(threads);

    ClearList(&list);

    printf ("Server stoped\n");

    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&pipe_mutex, NULL);

    struct ServerData data;
    bool finish = false;
    bool inf_mode = false;
    data.finish = &finish;
    data.inf_mode = &inf_mode;
    data.threads_count = atoi(argv[1]);

    pthread_t thread;
    int error = pthread_create(&thread, NULL, Server, (void*)(&data));

    if(error)
        printf("Server thread creation error!\n");

    while(!finish)
    {
        char command[5];
        char *pos;
        fgets(command, sizeof(command), stdin);
        if ((pos = strchr(command, '\n')) != NULL)
            *pos = '\0';
        if(strcmp(command, "stop") == 0)
        {
            finish = true;
        }
        else if(strcmp(command, "inf") == 0)
        {
            inf_mode = !inf_mode;
        }
    }

    void *status;
    pthread_join(thread, &status);


    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&pipe_mutex);

    return 0;
}
*/

