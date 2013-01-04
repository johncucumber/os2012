#define FS_FILE_PATH "/home/kirill/coding/MyPy/os2012/fuse/cfs_support/binfile"
#define MAX_FILE_PATH_SIZE 255
#define MAX_NODES 100
#define BLOCK_SIZE 4096

struct filestruct
{
    char path[MAX_FILE_PATH_SIZE];
    long size;
    char type;//0 - file 1 - directory 2 - symlink
    char exists;//0 - no 1 - yes
    long offset;
};

void initFileSystem();
struct filestruct *getNodes();
int getNumByPath(const char *path, struct filestruct *nodes);
void writeNode(struct filestruct node, int pos);
int Rename(const char *path, const char *newpath);
int createNode(const char *path, mode_t mode);
