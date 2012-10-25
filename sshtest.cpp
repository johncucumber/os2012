#include "ComandExcuter.h"

int main(int argc, char** argv )
{
	if (argc != 2)
	{
		printf("choose comand\n");
		return 0;
	}	
	PrintComResult(Execute(argv[1]));
	return 0;
}
