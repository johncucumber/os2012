#include "ComandExcuter.h"

FILE *Execute(char *cmd)
{
        return popen(cmd, "r");
}

void PrintComResult(FILE *ptr)
{
	if (ptr != NULL)
	{
		char buf[BUFSIZ];
		while (fgets(buf, BUFSIZ, ptr) != NULL)
			(void) printf("~>%s", buf);
		(void) pclose(ptr);
	}
}

