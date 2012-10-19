
#ifndef stringworkH
#define stringworkH
#define BUF_SIZE 512

#include <stdio.h>

char *readstring(int fd)
{
	char *res = (char*)malloc(sizeof(char) * BUF_SIZE);
	char buf = 1;
	int i = 0;
	while (buf != '\0')
	{
		read(fd, &buf, sizeof buf);
		//printf("recieve char %c\n", buf);
		if (i > 4)
			return res;
		res[i++] = buf;
	}
	return res;
}

void writestring(int fd, char* string)
{
	int i = 0;
	while (string[i] != '\0')
	{
		//printf("send char %c\n", string[i]);
		write(fd, &string[i++], sizeof(char));
	}
	write(fd, &string[i], sizeof(char));
}

#endif
