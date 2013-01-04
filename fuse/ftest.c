#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "fswork.h"

int main(int argc, char **argv)
{
    printf("Start....\n");
    struct filestruct *nodes = getNodes();
    printf("nd %s\n", nodes[0].path);
    strcpy(nodes[0].path, "/Yeah");
    writeNode(nodes[0], 0);
    nodes = getNodes();
    printf("nd %s\n", nodes[0].path);
    printf("Finish\n");
    return 0;
}
