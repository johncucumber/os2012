#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "fswork.h"

int main(int argc, char **argv)
{
    printf("Start....\n");
    struct filestruct *nodes = getNodes();
    printf("name %s s %ld\n", nodes[0].path, nodes[0].size);
    char *buf = malloc(nodes[0].size);
    long realsize = readFile(nodes[0], buf, 0, nodes[0].size);
    printf("realsize %ld\n", realsize);
    printf("Finish\n");
    return 0;
}
