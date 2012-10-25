#include <stdio.h>
#include <stdlib.h>
#ifndef CE_H
#define CE_H

FILE *Execute(char *cmd);
void PrintComResult(FILE *ptr);

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


#endif
