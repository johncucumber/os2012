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
    printf("nd %s\n", nodes[1].path);
    printf("nd %s\n", nodes[2].path);
    printf("0 %d %ld\n", isNodeLast(nodes[0]), nodes[0].offset);
    printf("1 %d %ld\n", isNodeLast(nodes[1]), nodes[1].offset);
    printf("2 %d %ld\n", isNodeLast(nodes[2]), nodes[2].offset);
    FILE *bfs = fopen(FS_FILE_PATH, "a+");
    long int noffset = ftell(bfs);
    printf("%ld\n", noffset);
    printf("Finish\n");
    return 0;
}
