#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "fswork.h"

int main(int argc, char **argv)
{
    printf("Start....\n");
    struct filestruct *nodes = getNodes();
    char buf[255];
    sprintf(buf, "num %d\n", 42);
    printf("%s <~\n", buf);
    printf("Finish\n");
    return 0;
}
