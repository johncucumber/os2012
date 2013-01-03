#include <stdio.h>
#include <stdlib.h>

#include "fswork.h"

void initFileSystem(void)  
{
    struct filestruct nodes[MAX_NODES];
    FILE *output;
    int i;
    if((output=fopen(FS_FILE_PATH, "a+"))==0)
    {
        puts ("Can't open fs's file.");
        exit(-1);
    }
    fwrite(nodes, 1, sizeof(nodes), output);
    /*for (i = 0; i < MAX_NODES; i++)
    {
        fwrite(output, nodes[i], sizeof(struct filestruct));
    }*/
    fclose(output);
}
