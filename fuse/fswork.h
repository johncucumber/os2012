#define FS_FILE_PATH "/home/kirill/coding/MyPy/os2012/fuse/cfs_support/binfile"
#define MAX_FILE_PATH_SIZE 255
#define MAX_NODES 100

struct filestruct
{
    char path[MAX_FILE_PATH_SIZE];
    int size;
    char type;//0 - file 1 - directory 2 - symlink
    char exists;//0 - no 1 - yes
};

void initFileSystem();
struct filestruct *getNodes();
int getNumByPath(const char *path, struct filestruct *nodes);
void writeNode(struct filestruct node, int pos);
int Rename(const char *path, const char *newpath);
