#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <semaphore.h>

#include "queue.h"

#define THC 64
#define PORT 1777
#define BUF_SIZE 512
sem_t queue_sem;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
struct Queue *queue;

void *performance();
void my_get(char *filename, int sock);

int main() {
    int sock, listener;
    sem_init(&queue_sem, 0, 0);
    struct sockaddr_in addr;
    queue = new_queue();

    int i;
    for (i = 0; i < THC; i++) {
        pthread_t thread;
        pthread_create(&thread, NULL, performance, NULL);
    }

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
        perror("bind");
        exit(2);
    }
    printf("THC=%d \n", THC, fflush);
    printf("SOMAXCONN=%d \n", SOMAXCONN, fflush);

    listen(listener, SOMAXCONN);
    int ci = 1;
    while (1) {
        sock = accept(listener, NULL, NULL);
        if (sock < 0) {
            perror("accept");
            exit(3);
        }
        printf("Request #%d \n", ci++, fflush);
        pthread_mutex_lock(&mymutex);
        push(queue, sock);
        pthread_mutex_unlock(&mymutex);
        sem_post(&queue_sem);
    }

    return 0;
}

void *performance() {
    while (1) {
        sem_wait(&queue_sem);
        pthread_mutex_lock(&mymutex);
        struct Node *param;
        param = pop(queue);
        pthread_mutex_unlock(&mymutex);

        int sock = param->sock;
        int bytes_read;
        char buf[BUF_SIZE];

        bytes_read = recv(sock, buf, BUF_SIZE, 0);
        if (bytes_read > 0) {
            my_get(buf, sock);
        }
        close(sock);
        printf("Close sock\n", fflush);
    }
}

void my_get(char *reqest, int sock) {   
    char delims[] = " \n";
    char *command = strtok(reqest, delims);
    char *filename = strtok(NULL, delims);
    char *protocol = strtok(NULL, delims);

    printf("Get request \"%s %s %s\" \n", command, filename, protocol, fflush);

    if (strcmp(command, "GET")) {
        printf("Non GET \n", fflush);
        return;
    }

    char def[] = "index.html";

    if (!strcmp(filename, "/"))
        filename = def;
    else
        filename = filename + 1;
    printf("Filename %s \n", filename, fflush);

    FILE *file;
    if ((file = fopen(filename, "r")) == NULL) {
        printf("404 Not Found\n", fflush);
        char header[] = "HTTP/1.1 404 Not Found\nServer: dimko/0.0.1\nContent-Type: text/html\nConnection: close\n\n";
        send(sock, header, sizeof (header), 0);
    } else {
        printf("200 OK\n", fflush);
        char header[] = "HTTP/1.1 200 OK\nServer: dimko/0.0.1\nContent-Type: text/html; charset=utf-8\nConnection: close\n\n";
        send(sock, header, sizeof (header), 0);
        char buf[BUF_SIZE];
        char c;
        int i = 0;
        printf("Start send file\n", fflush);
        memset(buf, 0, sizeof (buf));
        while ((c = fgetc(file)) != EOF) {
            buf[i] = c;
            i++;
            if ((i == BUF_SIZE - 1)) {
                send(sock, buf, sizeof (buf), 0);
                i = 0;
                memset(buf, 0, sizeof (buf));
            }
        }
        if (i != 0) {
            send(sock, buf, sizeof (buf), 0);
        }
        printf("File was sended\n", fflush);
        fclose(file);
    }
}
