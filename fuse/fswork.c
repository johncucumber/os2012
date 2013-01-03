#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fswork.h"
#include "log.h"

void initFileSystem(void)  
{
    //struct filestruct nodes[MAX_NODES];
    struct filestruct *nodes = malloc(sizeof(struct filestruct)*MAX_NODES);
    memset(nodes, 0, sizeof(struct filestruct)*MAX_NODES);
    FILE *output;
    if((output=fopen(FS_FILE_PATH, "wb")) == NULL)
    {
        puts ("Can't open fs's file.");
        exit(-1);
    }
    fwrite(nodes, 1, sizeof(struct filestruct)*MAX_NODES, output);
    fclose(output);
}

struct filestruct *getNodes()
{
    struct filestruct *nodes = malloc(sizeof(struct filestruct)*MAX_NODES);
    FILE *input;
    int i;
    if((input=fopen(FS_FILE_PATH, "rb")) == NULL)
    {
        puts ("Can't open fs's file.");
        exit(-1);
    }
    addLog("file opens");
    fread(nodes, sizeof(struct filestruct), MAX_NODES, input);

    fclose(input);

    return nodes;
}

