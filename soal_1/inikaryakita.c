#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

// Path to the directory
static const char *directory_path = "/home/ubuntu/SISOP/modul4/portofolio";

// Custom structure to hold file information
struct CustomFile {
    char name[256];
    mode_t mode;
};

// Function to get file attributes
static int custom_getattr(const char *path, struct stat *st) {
    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", directory_path, path);

    // Custom logic for getting file attributes
    int result = lstat(full_path, st);
    if (result == -1)
        return -errno;
    return 0;
}

// Function to read directory contents
static int custom_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", directory_path, path);

    DIR *dir = opendir(full_path);
    if (dir == NULL)
        return -errno;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = entry->d_ino;
        st.st_mode = entry->d_type << 12;

        // Custom logic for reading directory contents
        if (filler(buf, entry->d_name, &st, 0))
            break;
    }

    closedir(dir);
    return 0;
}

// Function to create a directory
static int custom_mkdir(const char *path, mode_t mode) {
    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", directory_path, path);

    // Custom logic for creating a directory
    int result = mkdir(full_path, mode);
    if (result == -1)
        return -errno;
    return 0;
}

// Function to rename a file or directory
static int custom_rename(const char *from, const char *to) {
    char from_path[1000], to_path[1000];
    snprintf(from_path, sizeof(from_path), "%s%s", directory_path, from);
    snprintf(to_path, sizeof(to_path), "%s%s", directory_path, to);

    // Custom logic for renaming
    int result = rename(from_path, to_path);
    if (result == -1)
        return -errno;

    // Apply watermark after renaming
    char watermark_command[2048];
    snprintf(watermark_command, sizeof(watermark_command), "convert %s -gravity south -geometry +0+20 -fill white -pointsize 36 -annotate +0+0 'inikaryakita.id' %s", to_path, to_path);
    system(watermark_command);

    return 0;
}

// Function to change file permissions
static int custom_chmod(const char *path, mode_t mode) {
    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", directory_path, path);

    int result = chmod(full_path, mode);
    if (result == -1)
        return -errno;
    return 0;
}

// Function to create a new file
static int custom_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", directory_path, path);

    // Custom logic for creating a file
    int fd = creat(full_path, mode);
    if (fd == -1)
        return -errno;
    fi->fh = fd;
    return 0;
}

// Function to delete a file
static int custom_unlink(const char *path) {
    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", directory_path, path);

    // Custom logic for deleting a file
    int result = unlink(full_path);
    if (result == -1)
        return -errno;
    return 0;
}

// Function to reverse the content of a buffer
static void reverse_buffer(char *buf, size_t size) {
    size_t i = 0;
    size_t j = size - 1;
    while (i < j) {
        char temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
        i++;
        j--;
    }
}

// Function to check if a string contains another string
static int contains_string(const char *haystack, const char *needle) {
    size_t haystack_len = strlen(haystack);
    size_t needle_len = strlen(needle);
    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        if (strncmp(haystack + i, needle, needle_len) == 0) {
            return 1;
        }
    }
    return 0;
}

// Custom logic for reading from a file
static int custom_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", directory_path, path);

    // Check if the filename contains the word "test"
    if (contains_string(path, "test")) {
        // Open the file
        int fd = open(full_path, O_RDONLY);
        if (fd == -1)
            return -errno;

        // Read from the file
        ssize_t result = pread(fd, buf, size, offset);
        if (result == -1) {
            close(fd);
            return -errno;
        }

        // Reverse the content of the buffer
        reverse_buffer(buf, result);

        // Close the file
        close(fd);
        return result;
    } else {
        // If the filename doesn't contain the word "test", read normally
        int fd = open(full_path, O_RDONLY);
        if (fd == -1)
            return -errno;
        ssize_t result = pread(fd, buf, size, offset);
        if (result == -1) {
            close(fd);
            return -errno;
        }
        close(fd);
        return result;
    }
}
    
// Function to write to a file
static int custom_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", directory_path, path);

    // Custom logic for writing to a file
    int fd = open(full_path, O_WRONLY);
    if (fd == -1)
        return -errno;
    ssize_t result = pwrite(fd, buf, size, offset);
    if (result == -1) {
        close(fd);
        return -errno;
    }
    close(fd);
    return result;
}

// Function to copy a file
static int custom_copy(const char *from, const char *to) {
    char from_path[1000], to_path[1000];
    snprintf(from_path, sizeof(from_path), "%s%s", directory_path, from);
    snprintf(to_path, sizeof(to_path), "%s%s", directory_path, to);

    // Custom logic for copying a file
    int source_fd = open(from_path, O_RDONLY);
    if (source_fd == -1)
        return -errno;

    int dest_fd = open(to_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (dest_fd == -1) {
        close(source_fd);
        return -errno;
    }

    char buffer[4096];
    ssize_t bytes_read, bytes_written;
    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            close(source_fd);
            close(dest_fd);
            return -errno;
        }
    }

    close(source_fd);
    close(dest_fd);
    return 0;
}

// Function to apply watermark
static int custom_watermark(const char *path) {
    char full_path[1000];
    snprintf(full_path, sizeof(full_path), "%s%s", directory_path, path);

    // Custom logic for applying watermark
    char watermark_command[2048];
    snprintf(watermark_command, sizeof(watermark_command), "convert %s -gravity south -geometry +0+20 -fill white -pointsize 36 -annotate +0+0 'inikaryakita.id' %s", full_path, full_path);
    system(watermark_command);

    return 0;
}

// Function to create a directory with watermark
static int custom_mkdir_watermark(const char *path, mode_t mode) {
    int result = custom_mkdir(path, mode);
    if (result != 0)
        return result;

    custom_watermark(path);
    return 0;
}

// Function to rename file or directory with watermark
static int custom_rename_watermark(const char *from, const char *to) {
    int result = custom_rename(from, to);
    if (result != 0)
        return result;

    custom_watermark(to);
    return 0;
}

// Fuse Operations Structure
static struct fuse_operations custom_operations = {
    .getattr    = custom_getattr,
    .readdir    = custom_readdir,
    .mkdir      = custom_mkdir_watermark,
    .rename     = custom_rename_watermark,
    .chmod      = custom_chmod,
    .create     = custom_create,
    .unlink     = custom_unlink,
    .read       = custom_read,
    .write      = custom_write,
};

// Main function
int main(int argc, char *argv[]) {
    return fuse_main(argc, argv, &custom_operations, NULL);
}
