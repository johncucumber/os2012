#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

//const int tableSize = 301; 
#define tableSize 301
const int step = 1; 
struct HashItem { char* direct; int id; int empty; int visit; }; 
struct HashTable { int size; struct HashItem arrayHash[tableSize]; };

int main(int argc, char** argv)
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(10026);
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);
	int i;
	if(result == -1) {
		perror("oops: client");
		exit(1);
	}
	char buf[512];
	write(sockfd, "123", 4);
	read(sockfd, buf, sizeof buf);
	if (!strcmp(buf, "NO"))
	{
		printf("goodby\n");
		return 0;
	}
	for (i = 0; i < 600; i++)
		buf[i] = 1;
	write(sockfd, buf, 512);
	read(sockfd, buf, sizeof buf);
	printf("leaving...\n");
	close(sockfd);
	exit(0);
}

