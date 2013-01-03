#include <stdio.h>
#include <stdlib.h>

#include "fswork.h"

void initFileSystem(void)  
{
    struct filestruct nodes[MAX_NODES];
    FILE *output;
    if((output=fopen(FS_FILE_PATH, "a+"))==0)
    {
        puts ("Can't open fs's file.");
        exit(-1);
    }
    fwrite(nodes, 1, sizeof(nodes), output);
    fclose(output);
}

struct filestruct *getNodes()
{
    struct filestruct *nodes = malloc(sizeof(struct filestruct)*MAX_NODES);
    FILE *input;
    int i;
    if((input=fopen(FS_FILE_PATH, "a+"))==0)
    {
        puts ("Can't open fs's file.");
        exit(-1);
    }


    fclose(input);

    return nodes;
}

