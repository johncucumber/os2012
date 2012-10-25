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
	char* command = "ls";
	if (argc> 1)
		command = argv[1];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(10010);

	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if(result == -1) {
		perror("oops: client1");
		exit(1);
	}
	char buf[512] = "Hi!!!";
	write(sockfd, command, strlen(command));
	read(sockfd, buf, sizeof buf);
	printf("reciv = %s\n", buf);
	
	close(sockfd);
	exit(0);
}

