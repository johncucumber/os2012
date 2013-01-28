#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define myPort 1488
#define BUF_SIZE 512
int main(int argc, char** argv)
{
    char buf[BUF_SIZE];
    char message[BUF_SIZE]="GET /1.txt HTTP/1.0";
  
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(myPort);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

  //  printf("%s",message);fflush(stdout); 
    send(sock, message, sizeof(message), 0);
    
    recv(sock, buf, sizeof(message), 0);printf("%s\n",buf);
    close(sock);
    return 0;
}
