#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

char message[] = "Hello there!\n";
char buf[1024], buf1[1024];
char mes[1024];

int main()
{	
    int i = -1;
    int c;
    /*while((c=getchar())!='\n'){
      //printf("%c",c);
      buf[++i] = c;
    }*/
    char buf[1024]="GET /index.html HTTP/1.0";
    int sock;
    struct sockaddr_in addr;
	//printf("%s\n", buf);
	//stdout(fflush);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1555); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    send(sock, buf, 1024, 0);
    recv(sock, buf, 1024, 0);
    printf("%s\n",buf);
	    return 0;
    recv(sock, buf, 1024, 0);

    printf("%s\n",buf);
    close(sock);

    return 0;
}
