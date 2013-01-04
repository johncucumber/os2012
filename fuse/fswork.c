#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fswork.h"
#include "log.h"

void initFileSystem(void)  
{
    struct filestruct *nodes = malloc(sizeof(struct filestruct)*MAX_NODES);
    memset(nodes, 0, sizeof(struct filestruct)*MAX_NODES);
    FILE *output;
    if((output=fopen(FS_FILE_PATH, "wb")) == NULL)
    {
        puts ("Can't open fs's file.");
        exit(-1);
    }
    int i;
    strcpy(nodes[0].path, "/Daivers fs\0");
    nodes[0].exists = 1;
    strcpy(nodes[2].path, "/some");
    nodes[2].exists = 1;
    fwrite(nodes, 1, sizeof(struct filestruct)*MAX_NODES, output);
    fclose(output);
    addLog("New fs was created");
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

struct filestruct *getByPath(const char *path, struct filestruct *nodes)
{
    int i;
    struct filestruct *res;
    for (i = 0; i < MAX_NODES; i++)
    {
        //const char *tmppath = path + 1;
        if ((nodes[i].exists) && (strcmp(path, nodes[i].path) == 0)) 
        {
            res = &nodes[i];
            return res;
        }
    }
    return NULL;
}
