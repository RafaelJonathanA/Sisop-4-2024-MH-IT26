#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

static  const  char *dirpath = "/home/abhi/sisop/sensitif";
const char *correct_password = "password123";

int entered_pass = 0;

// Function to log operations
void log_operation(const char *status, const char *tag, const char *info) {
    FILE *log_file = fopen("logs-fuse.log", "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%d/%m/%Y-%H:%M:%S", t);

    fprintf(log_file, "[%s]::%s::[%s]::[%s]\n", status, time_str, tag, info);
    fclose(log_file);
}


// Helper function to prompt for password
int prompt_password() {
    char input_password[256];
    printf("Enter password to access this folder: ");
    if (scanf("%255s", input_password) != 1) {
        return 0; // Failed to read input
    }
    return strcmp(input_password, correct_password) == 0;
}

// Helper function to decode base64
char *base64_decode(const char *data, size_t *out_len) {
    BIO *b64, *bmem;
    size_t len = strlen(data);
    char *buffer = malloc(len);
    memset(buffer, 0, len);

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new_mem_buf((void *)data, len);
    bmem = BIO_push(b64, bmem);

    *out_len = BIO_read(bmem, buffer, len);
    buffer[*out_len] = '\0';

    BIO_free_all(bmem);
    return buffer;
}

// Helper function to decode rot13
void rot13_decode(char *data) {
    for (int i = 0; data[i]; i++) {
        if (('a' <= data[i] && data[i] <= 'z') || ('A' <= data[i] && data[i] <= 'Z')) {
            if ((data[i] >= 'a' && data[i] <= 'm') || (data[i] >= 'A' && data[i] <= 'M')) {
                data[i] += 13;
            } else {
                data[i] -= 13;
            }
        }
    }
}

// Helper function to decode hex
char *hex_decode(const char *data, size_t *out_len) {
    size_t len = strlen(data);

    *out_len = len / 2;
    char *buffer = malloc(*out_len);

    for (size_t i = 0; i < *out_len; i++) {
        sscanf(data + 2 * i, "%2hhx", &buffer[i]);
    }
    return buffer;
}


// Helper function to reverse a string
void reverse(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

static int pesan_getattr(const char *path, struct stat *stbuf) {
    int res = 0;
    char full_path[512];
    sprintf(full_path, "%s%s", dirpath, path);

    res = lstat(full_path, stbuf);
    if (res == -1){
        log_operation("FAILED", "getattr", strerror(errno));
        return -errno;
    }
        
    log_operation("SUCCESS", "getattr", path);
    return 0;
}

static int pesan_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    DIR *dp;
    struct dirent *de;
    char full_path[512];

    (void) offset;
    (void) fi;

    sprintf(full_path, "%s%s",dirpath, path);

    dp = opendir(full_path);
    if (dp == NULL){
        log_operation("FAILED", "readdir", strerror(errno));
        return -errno;
    }
        

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    closedir(dp);
    log_operation("SUCCESS", "readdir", path);
    return 0;
}

static int pesan_open(const char *path, struct fuse_file_info *fi) {
    int res;
    char full_path[512];
    sprintf(full_path, "%s%s",dirpath, path);

    if (strncmp(path, "/rahasia-berkas", 15) == 0 && entered_pass == 0) {
        if (!prompt_password()) {
            log_operation("FAILED", "open", "Access denied to rahasia-berkas");
            return -EACCES;
        } else {
            entered_pass = 1;
        }
    }

    res = open(full_path, fi->flags);
    if (res == -1){
        log_operation("FAILED", "open", strerror(errno));
        return -errno;
    }

    close(res);
    log_operation("SUCCESS", "open", path);
    return 0;
}

static int pesan_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    int fd;
    int res;
    char full_path[512];
    sprintf(full_path, "%s%s",dirpath, path);

    fd = open(full_path, O_RDONLY);
    if (fd == -1){
        log_operation("FAILED", "read", strerror(errno));
        return -errno;
    }
        

    // Read the entire file into a buffer
    struct stat st;
    fstat(fd, &st);
    size_t file_size = st.st_size;
    char *file_buf = malloc(file_size + 1);
    if (!file_buf) {
        close(fd);
        log_operation("FAILED", "read", "Memory allocation failed");
        return -ENOMEM;
    }

    res = pread(fd, file_buf, file_size, 0);
    if (res == -1) {
        free(file_buf);
        close(fd);
        log_operation("FAILED", "read", strerror(errno));
        return -errno;
    }
    file_buf[file_size] = '\0';

    // Check for prefixes in the filename and decode accordingly
    if (strstr(path, "base64") != NULL) {
        size_t decoded_len;
        char *decoded_buf = base64_decode(file_buf, &decoded_len);
        if (decoded_buf) {
            if (offset < decoded_len) {
                if (offset + size > decoded_len)
                    size = decoded_len - offset;
                memcpy(buf, decoded_buf + offset, size);
                res = size;
            } else {
                res = 0;
            }
            free(decoded_buf);
        } else {
            log_operation("FAILED", "read", "Base64 decode failed");
            res = -EINVAL;
        }
    } else if (strstr(path, "rot13") != NULL) {
        rot13_decode(file_buf);
        size_t len = strlen(file_buf);
        if (offset < len) {
            if (offset + size > len)
                size = len - offset;
            memcpy(buf, file_buf + offset, size);
            res = size;
        } else {
            res = 0;
        }
    } else if (strstr(path, "hex") != NULL) {
        size_t decoded_len;
        char *decoded_buf = hex_decode(file_buf, &decoded_len);
        printf("%s", decoded_buf);
        if (decoded_buf) {
            if (offset < decoded_len) {
                if (offset + size > decoded_len)
                    size = decoded_len - offset;
                memcpy(buf, decoded_buf + offset, size);
                res = size;
            } else {
                printf("di didalam\n");
                res = 0;
            }
            free(decoded_buf);
        } else {
            printf("di awal\n");
            log_operation("FAILED", "read", "Hex decode failed");
            res = -EINVAL;
        }
    } else if (strstr(path, "rev") != NULL) {
        reverse(file_buf);
        size_t len = strlen(file_buf);
        if (offset < len) {
            if (offset + size > len)
                size = len - offset;
            memcpy(buf, file_buf + offset, size);
            res = size;
        } else {
            res = 0;
        }
    } else {
        if (offset < file_size) {
            if (offset + size > file_size)
                size = file_size - offset;
            memcpy(buf, file_buf + offset, size);
            res = size;
        } else {
            res = 0;
        }
    }

    free(file_buf);
    close(fd);
    log_operation("SUCCESS", "read", path);
    return res;
}


// Adding opendir function
static int pesan_opendir(const char *path, struct fuse_file_info *fi) {
    DIR *dp;
    char full_path[512];
    sprintf(full_path, "%s%s",dirpath, path);

    // Check for "rahasia-berkas" prefix
    if (strncmp(path, "/rahasia-berkas", 15) == 0 && entered_pass == 0) {
        if (!prompt_password()) {
            log_operation("FAILED", "opendir", "Access denied to rahasia-berkas");
            return -EACCES;
        } else {
            entered_pass = 1;
        }
    }

    dp = opendir(full_path);
    if (dp == NULL){
        log_operation("FAILED", "opendir", strerror(errno));
        return -errno;
    }
        

    closedir(dp);
    log_operation("SUCCESS", "opendir", path);
    return 0;
}

// Adding releasedir function to reset authentication
static int pesan_releasedir(const char *path, struct fuse_file_info *fi) {
    // Check for "rahasia-berkas" prefix
    if (strncmp(path, "/rahasia-berkas", 15) != 0) {
        entered_pass = 0;
    }
    log_operation("SUCCESS", "releasedir", path);
    return 0;
}

static struct fuse_operations pesan_oper = {
    .getattr    = pesan_getattr,
    .readdir    = pesan_readdir,
    .opendir    = pesan_opendir,
    .open       = pesan_open,
    .read       = pesan_read,
    .releasedir = pesan_releasedir
};

int main(int argc, char *argv[]) {
    return fuse_main(argc, argv, &pesan_oper, NULL);
}
