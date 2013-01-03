/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall hello.c `pkg-config fuse --cflags --libs` -o hello
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "log.h"
#include "fswork.h"

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";

static int cfs_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
        return res;
	}     
    int i;
    struct filestruct *nodes = getNodes();
    for (i = 0; i < MAX_NODES; i++)
    {
        const char *tmppath = path + 1;
        if ((nodes[i].exists) && (strcmp(tmppath, nodes[i].path) == 0)) 
        {
            stbuf->st_mode = S_IFREG | 0444;
            stbuf->st_nlink = 1;
            stbuf->st_size = strlen(hello_str);
            addLog("path exists");
            return res;
        }
    }
    
	res = -ENOENT;

	return res;
}

static int cfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

    struct filestruct *nodes = getNodes();
    int i;
    for (i = 0 ; i < MAX_NODES; i++)
    {
        if (nodes[i].exists)
            filler(buf, nodes[i].path, NULL, 0);
    }

	return 0;
}

static int cfs_open(const char *path, struct fuse_file_info *fi)
{
	if (strcmp(path, hello_path) != 0)
		return -ENOENT;

	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;

	return 0;
}

static int cfs_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	size_t len;
	(void) fi;
	if(strcmp(path, hello_path) != 0)
		return -ENOENT;

	len = strlen(hello_str);
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, hello_str + offset, size);
	} else
		size = 0;

	return size;
}


static void* cfs_init(struct fuse_conn_info *conn)
{
}

static struct fuse_operations cfs_oper = {
	.getattr	= cfs_getattr,//
	.readdir	= cfs_readdir,//
	.open		= cfs_open,//
	.read		= cfs_read,//
    .init       = cfs_init,
};

int main(int argc, char *argv[])
{
    addLog("Start work");
    initFileSystem();
    //struct filestruct *nodes = getNodes();
	return fuse_main(argc, argv, &cfs_oper, NULL);
}
