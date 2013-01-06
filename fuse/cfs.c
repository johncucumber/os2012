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
    int nd = getNumByPath(path, nodes); 
    if (nd != -1)
    {
        stbuf->st_ino = nd;
        //stbuf->st_mode = n.di_mode;    /* protection */
        //stbuf->st_nlink = n.di_nlink;   /* number of hard links */
        stbuf->st_uid = nodes[nd].uid;
        stbuf->st_gid =  nodes[nd].gid;
        stbuf->st_atime = nodes[nd].atime;
        stbuf->st_mtime = nodes[nd].mtime;
        stbuf->st_ctime = nodes[nd].ctime;

        stbuf->st_mode = nodes[nd].mode;//S_IFREG | 0777;
        stbuf->st_nlink = nodes[nd].n_link;
        stbuf->st_size = nodes[nd].size;//strlen(hello_str);
        //addLog("path exists");
        return res;

    }
    //spike    
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

    //add selflink 
	filler(buf, ".", NULL, 0);
    //add parent link
	filler(buf, "..", NULL, 0);

    //cut my arms :)
    struct filestruct *nodes = getNodes();
    int i;
    for (i = 0 ; i < MAX_NODES; i++)
    {
        if (nodes[i].exists)
        {
            const char *tmppath = nodes[i].path + 1;
            filler(buf, tmppath, NULL, 0);
        }
    }

	return 0;
}

static int cfs_open(const char *path, struct fuse_file_info *fi)
{
    struct filestruct *nodes = getNodes();
    int nd = getNumByPath(path, nodes); 
    fi->fh = nd;
    if (nd > -1)
    {
        return 0;
    }
    return -ENOENT;
}

static int cfs_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
    struct filestruct *nodes = getNodes();
    int nd = getNumByPath(path, nodes); 
    if(nd < 0)
    {
        return -ENOENT;
    }
    if((size = readFile(nodes[nd], (void *)buf, (long)offset, size)) < 0)
    {
        return -EIO;
    }
    char sbuf[1024];
    sprintf(sbuf, "real read %ld", size);
    addLog(sbuf);
	return size;
}

int cfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    struct filestruct *nodes = getNodes();
    int nd = getNumByPath(path, nodes); 
    if(nd < 0)
    {
        return -ENOENT;
    }
    if((size = writeFile(nodes[nd], (void *)buf, (long)offset, size, nd)) < 0)
    {
        return -EIO;
    }
    return size;
}

int cfs_rename(const char *path, const char *newpath)
{
    //return 0;
    addLog("Renaming...");
    addLog(newpath);
    return Rename(path, newpath);
}

static void* cfs_init(struct fuse_conn_info *conn)
{
    addLog("Start work");
    //initFileSystem();
}

int cfs_mknod(const char *path, mode_t mode, dev_t dev)
{
    if (S_ISREG(mode))
        return createNode(path, mode, 0, NULL);

    return -EINVAL;
}

int cfs_unlink(const char *path)
{
    struct filestruct *nodes = getNodes();
    int nd = getNumByPath(path, nodes); 
    if (nd == -1)
    {
        return -ENOENT;
    }
    nodes[nd].exists = 0;
    writeNode(nodes[nd], nd);
    return 0;
}

int symlink(const char *path, const char *link)
{
     struct filestruct *nodes = getNodes();
    int nd = getNumByPath(path, nodes); 
    if (nd == -1)
    {
        return -EINVAL;
    }
    
}


static struct fuse_operations cfs_oper = {
	.getattr	= cfs_getattr,//
	.readdir	= cfs_readdir,//
	.open		= cfs_open,//
	.read		= cfs_read,//
    .write      = cfs_write,
    .init       = cfs_init,
    .rename     = cfs_rename,
    .mknod      = cfs_mknod,
    .unlink     = cfs_unlink,
};

int main(int argc, char *argv[])
{
    //struct filestruct *nodes = getNodes();
	return fuse_main(argc, argv, &cfs_oper, NULL);
}
