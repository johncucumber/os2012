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
	address.sin_port = htons(10023);
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if(result == -1) {
		perror("oops: client");
		exit(1);
	}
	char buf[512];
	
	
	if (argc> 1)
	{
		char *command = argv[1];
		write(sockfd, command, strlen(command));
		read(sockfd, buf, sizeof buf);
		printf("reciv = %s\n", buf);
		write(sockfd, "ls", 3);
		read(sockfd, buf, sizeof buf);
		printf("reciv2 = %s\n", buf);
	}	
	else
	{
		int len = 1;
		while ((len != 0))
		{
			char command[80];
			printf("~>");
			//scanf("%s", command);
			gets(command);
			len = strlen(command);
			//printf("before sending...\n");			
			write(sockfd, command, len);
			read(sockfd, buf, sizeof buf);
			printf("<~ %s\n", buf);			
		}
	}
	printf("leaving...\n");
	close(sockfd);
	exit(0);
}

