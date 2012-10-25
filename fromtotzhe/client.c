#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


struct Message
{
    char sender[32];
    char text[1024];
    char text1[32000];
    char text2[30000];
};

int main(int argc, char *argv[])
{
    int i = 0;
    int pid;

    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3426); // порт
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    int count = atoi(argv[1]);
    while(i < count)
    {
        struct Message message;
        int size;
        size = sprintf(message.text, "%s", "Hello! This is ");
        size +=  sprintf(message.text + size, "%i", getpid());
        size +=  sprintf(message.text + size, "%s", "\n");

        size = sprintf(message.sender, "%i", getpid());

        printf("Process sending message: %s\n", message.text);
        int n = send(sock, &message, sizeof(message), 0);
        printf("Sent %d of %d bytes\n", n, sizeof(message));
        recv(sock, &message, sizeof(message), 0);

        printf("%s: %s\n", message.sender, message.text);
        i++;
    }
    close(sock);

    return 0;
}
