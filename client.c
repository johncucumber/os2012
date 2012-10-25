#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include "stringwork.h"


char *login = "admin";
char *pass = "123";


int main(int argc, char** argv)
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch = 'A';
	if (argc == 2)
		ch = argv[1][0];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(10003);

	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if(result == -1) {
		perror("oops: client1");
		exit(1);
	}
	char buf[512] = "Hi!";
	write(sockfd, "Hi!!", 5);
	read(sockfd, buf, sizeof buf);
	printf("reciv = %s\n", buf);
	write(sockfd, buf, sizeof buf);
	read(sockfd, buf, sizeof buf);
	printf("reciv2 = %s\n", buf);
	close(sockfd);
	exit(0);
}

