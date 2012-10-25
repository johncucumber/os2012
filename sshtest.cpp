#include <stdio.h>
#include <stdlib.h>

FILE *Excute(char *cmd)
{
        return popen(cmd, "r");
}

void PrintResult(FILE *ptr)
{
	if (ptr != NULL)
	{
		char buf[BUFSIZ];
		while (fgets(buf, BUFSIZ, ptr) != NULL)
			(void) printf("%s", buf);
		(void) pclose(ptr);
	}
}

int main(int argc, char** argv )
{
	if (argc != 2)
	{
		printf("choose comand\n");
		return 0;
	}	
	PrintResult(Excute(argv[1]));
	return 0;
}
