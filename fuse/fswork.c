#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

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

int getNumByPath(const char *path, struct filestruct *nodes)
{
    int i;
    for (i = 0; i < MAX_NODES; i++)
    {
        //const char *tmppath = path + 1;
        if ((nodes[i].exists) && (strcmp(path, nodes[i].path) == 0)) 
        {
            return i;
        }
    }
    return -1;
}

void writeNode(struct filestruct node, int pos)
{
    FILE *fl;
    int i;
    if((fl=fopen(FS_FILE_PATH, "rb+")) == NULL)
    {
        puts ("Can't open fs's file.");
        exit(-1);
    }
    fseek(fl, pos*sizeof(struct filestruct), 0);
    fwrite(&node, 1, sizeof(struct filestruct), fl);
    fclose(fl);
}

int Rename(const char *path, const char *newpath)
{
    struct filestruct *nodes = getNodes();
    int ind = getNumByPath(path, nodes);
    if (ind < 0) return -ENOENT;
    strcpy(nodes[ind].path, newpath);
    writeNode(nodes[ind], ind);
    return 0;
}

int createNode(const char *path, mode_t mode)
{
    struct filestruct *nodes = getNodes();
    int ind = getNumByPath(path, nodes);
    if (ind > -1)
    {
        return -EEXIST;
    }
    int i, res;
    for (i = 0; i < MAX_NODES; i++)
    {
        if (nodes[i].exists == 0)
        {
            res = i;
            break;
        }
    }
    strcpy(nodes[i].path, path);
    FILE *bfs = fopen(FS_FILE_PATH, "a+");
    fseek(bfs, 0L, SEEK_END);
    int noffset = ftell(bfs);
    nodes[i].offset = noffset;
    nodes[i].exists = 1;
    nodes[i].size = 0;//BLOCK_SIZE
    fclose(bfs);
    writeNode(nodes[i], i);
    return 0;
}

char isNodeLast(struct filestruct node)
{
    FILE *fp;
    if((fp=fopen(FS_FILE_PATH, "rb")) == NULL)
    {
        puts ("Can't open fs's file.");
        exit(-1);
    }
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    return node.size + node.offset == sz;
}

long writeFile(struct filestruct node, void *buf, long offset, long size)
{
    FILE *fl;
    int i;
    if((fl=fopen(FS_FILE_PATH, "rb+")) == NULL)
    {
        puts ("Can't open fs's file.");
        exit(-1);
    }
    fseek(fl, node.offset + offset, 0);
    fwrite(buf, 1, size, fl);
    node.size += size;
    fclose(fl);

}

int readFile(struct filestruct node, char *buf, long offset, long size)
{
    FILE *fl;
    int i;
    if((fl=fopen(FS_FILE_PATH, "rb+")) == NULL)
    {
        puts ("Can't open fs's file.");
        exit(-1);
    }
    fseek(fl, node.offset + offset, 0);
    fread(buf, 1, size, fl);
    fclose(fl);
    return 0;
}

