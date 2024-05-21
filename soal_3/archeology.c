#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static const char *relics_path = "/home/ubuntu/sisop/relics";
static const size_t CHUNK_SIZE = 10240; // 

static void get_fullpath(char *fullpath, const char *path, int chunk) {
    if (chunk >= 0)
        sprintf(fullpath, "%s%s.%03d", relics_path, path, chunk);
    else
        sprintf(fullpath, "%s%s", relics_path, path);
}

static int relic_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else {
        char fullpath[1024];
        get_fullpath(fullpath, path, 0);
        res = stat(fullpath, stbuf);
        if (res == -1) {
            return -errno;
        }
       
        stbuf->st_size = 0;
        for (int i = 0;; i++) {
            get_fullpath(fullpath, path, i);
            struct stat st;
            if (stat(fullpath, &st) == -1)
                break;
            stbuf->st_size += st.st_size;
        }
    }
    return 0;
}

static int relic_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    DIR *dp;
    struct dirent *de;

    (void) offset;
    (void) fi;

    if (strcmp(path, "/") != 0)
        return -ENOENT;

    dp = opendir(relics_path);
    if (dp == NULL)
        return -errno;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    while ((de = readdir(dp)) != NULL) {
        if (de->d_name[0] == '.') continue;

        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (strstr(de->d_name, ".000") != NULL) {
            char base_name[256];
            strncpy(base_name, de->d_name, strlen(de->d_name) - 4);
            base_name[strlen(de->d_name) - 4] = '\0';
            filler(buf, base_name, &st, 0);
        }
    }
    closedir(dp);
    return 0;
}

static int relic_open(const char *path, struct fuse_file_info *fi)
{
    char fullpath[1024];
    get_fullpath(fullpath, path, 0);

    int fd = open(fullpath, fi->flags);
    if (fd == -1)
        return -errno;

    close(fd);
    return 0;
}

static int relic_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    (void) fi;
    size_t total_read = 0;
    int chunk = offset / CHUNK_SIZE;
    off_t chunk_offset = offset % CHUNK_SIZE;

    while (size > 0) {
        char fullpath[1024];
        get_fullpath(fullpath, path, chunk);

        FILE *file = fopen(fullpath, "rb");
        if (file == NULL)
            break;

        fseek(file, chunk_offset, SEEK_SET);
        size_t bytes_to_read = CHUNK_SIZE - chunk_offset;
        if (bytes_to_read > size)
            bytes_to_read = size;

        size_t bytes_read = fread(buf + total_read, 1, bytes_to_read, file);
        fclose(file);

        if (bytes_read <= 0)
            break;

        total_read += bytes_read;
        size -= bytes_read;
        chunk++;
        chunk_offset = 0;
    }

    if (total_read == 0)
        return -errno;

    return total_read;
}

static int relic_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    (void) fi;
    size_t total_written = 0;
    int chunk = offset / CHUNK_SIZE;
    off_t chunk_offset = offset % CHUNK_SIZE;

    while (size > 0) {
        char fullpath[1024];
        get_fullpath(fullpath, path, chunk);

        FILE *file = fopen(fullpath, "r+b");
        if (file == NULL) {
            file = fopen(fullpath, "wb");
            if (file == NULL)
                return -errno;
        }

        fseek(file, chunk_offset, SEEK_SET);
        size_t bytes_to_write = CHUNK_SIZE - chunk_offset;
        if (bytes_to_write > size)
            bytes_to_write = size;

        size_t bytes_written = fwrite(buf + total_written, 1, bytes_to_write, file);
        fclose(file);

        if (bytes_written <= 0)
            return -errno;

        total_written += bytes_written;
        size -= bytes_written;
        chunk++;
        chunk_offset = 0;
    }

    return total_written;
}

static int relic_unlink(const char *path)
{
    char fullpath[1024];
    for (int i = 0;; i++) {
        get_fullpath(fullpath, path, i);
        int res = unlink(fullpath);
        if (res == -1)
            break;
    }
    return 0;
}

static struct fuse_operations relic_oper = {
    .getattr    = relic_getattr,
    .readdir    = relic_readdir,
    .open       = relic_open,
    .read       = relic_read,
    .write      = relic_write,
    .unlink     = relic_unlink,
};

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <mountpoint>\n", argv[0]);
        return 1;
    }
    
    char *fuse_argv[argc + 2];
    for (int i = 0; i < argc; i++) {
        fuse_argv[i] = argv[i];
    }
    fuse_argv[argc] = "-o";
    fuse_argv[argc + 1] = "nonempty";
    fuse_argv[argc + 2] = NULL;

    return fuse_main(argc + 2, fuse_argv, &relic_oper, NULL);
}
