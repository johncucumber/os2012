#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>

#include "log.h"
#include "fswork.h"

int main(int argc, char **argv)
{
    printf("Start....\n");
    struct filestruct *nodes = getNodes();
    printf("time %d\n", time(NULL));
    printf("symb %s %d %s \n", nodes[1].path, nodes[1].type, nodes[1].link); 
    printf("Finish\n");
    return 0;
}
