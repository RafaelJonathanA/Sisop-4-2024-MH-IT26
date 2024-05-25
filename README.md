# Sisop-4-2024-MH-IT26

## ***KELOMPOK IT 26***
  | Nama      | NRP         |
  |-----------|-------------|
  | Rafael Jonathan Arnoldus   | 5027231006  |
  | Muhammad Abhinaya Al-Faruqi  | 5027231011  |  
  | Danendra Fidel Khansa  | 5027231063  |

## Bagian Pengerjaan Soal 
+ Soal 1 = Rafael Jonathan Arnoldus
+ Soal 2 = Muhammad Abhinaya Al-Faruqi
+ Soal 3 = Danendra Fidel Khansa

## ***MODUL 4***
  Berikut untuk struktur repository pengumpulan dari hasil praktikum sistem operasi modul 4 :
```
—soal_1:
        — inikaryakita.c
                                    
—soal_2:
        — pastibisa.c
				
—soal_3:
        — archeology.c
```				

## ***SOAL 1 (Rafael)***

Adfi merupakan seorang CEO agency creative bernama Ini Karya Kita. Ia sedang melakukan inovasi pada manajemen project photography Ini Karya Kita. Salah satu ide yang dia kembangkan adalah tentang pengelolaan foto project dalam sistem arsip Ini Karya Kita. Dalam membangun sistem ini, Adfi tidak bisa melakukannya sendirian, dia perlu bantuan mahasiswa Departemen Teknologi Informasi angkatan 2023 untuk membahas konsep baru yang akan mengubah project fotografinya lebih menarik untuk dilihat. Adfi telah menyiapkan portofolio hasil project fotonya yang bisa didownload dan diakses di `www.inikaryakita.id` . Silahkan eksplorasi web Ini Karya Kita dan temukan halaman untuk bisa mendownload projectnya. Setelah kalian download terdapat folder **gallery** dan **bahaya**.

- Pada folder “**gallery**”:
  + Membuat folder dengan prefix "**wm**." Dalam folder ini, setiap gambar yang dipindahkan ke dalamnya akan diberikan watermark bertuliskan **inikaryakita.id**.

**Ex**: "mv ikk.jpeg wm-foto/" 
```
Output: 
Before: (tidak ada watermark bertuliskan inikaryakita.id)
```
![Cuplikan layar 2024-05-23 193329](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/ca946189-bc39-41e9-bda0-59fab43b80bd)
```
After: (terdapat watermark tulisan inikaryakita.id)
```
![Cuplikan layar 2024-05-23 193336](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/bae8ca21-7743-495e-8d1d-a3b79d668bf1)

- Pada folder "**bahaya**," terdapat file bernama "script.sh." Adfi menyadari pentingnya menjaga keamanan dan integritas data dalam folder ini. 

  + Mereka harus mengubah permission pada file "script.sh" agar bisa dijalankan, karena jika dijalankan maka dapat menghapus semua dan isi dari  "gallery"
  + Adfi dan timnya juga ingin menambahkan fitur baru dengan membuat file dengan prefix "test" yang ketika disimpan akan mengalami pembalikan (reverse) isi dari file tersebut.

 ## ***PENGERJAAN***
### inikaryakita.c 
```c 
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

    // Check if file is script.sh or not 
    char *script_name = "script.sh";
    if (strcmp(path, script_name) == 0) {
        // Add execution to script.sh 
        mode_t execute_mode = mode | S_IXUSR | S_IXGRP | S_IXOTH;
        if (chmod(full_path, execute_mode) == -1)
            return -errno;
    }

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

```

## ***PENJELASAN PENGERJAAN***

Kode diatas memiliki beberapa fungsi yaitu :
- Membaca isi direktori
- Membuat direktori
- Mengubah nama file dan direktori
- Mengubah izin file
- Membuat file baru
- Menghapus file
- Membaca dari file (dengan pembalikan konten untuk file yang mengandung "test" dalam namanya)
- Menulis ke file
- Menyalin file
- Mengeksekusi logika khusus untuk nama file tertentu (misalnya, `script.sh`) yang akan langsung memberikan permission untuk script.sh di eksekusi 

```c
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
```
Kode diatas berfungsi 

Untuk membangun path lengkap dengan menggabungkan directory_path dan path yang diberikan. Yaitu home/ubuntu/SISOP/modul4/Portofolio 
Menggunakan lstat, kami mengambil atribut berkas dari path yang ditentukan dan menyimpannya dalam struktur st.
Jika pengambilan berhasil, fungsi mengembalikan 0. Jika tidak, ia mengembalikan nomor kesalahan yang sesuai.

```c
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
```
Kode diatas berfungsi untuk membaca konten dari direktori yang sudah di cari pada kode diatasnya lalu memasukkan berkas yanng diterima ke buffer. Apabila direktori tidak dapat dibuka maka akan ada pesan error yang keluar, lalu saat membaca direktori hasilnya tidak null maka akan ada pointer yang menunjuk ke buffer setelah itu direktori akan di tutup. 


```c
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

    // Check if file is script.sh or not 
    char *script_name = "script.sh";
    if (strcmp(path, script_name) == 0) {
        // Add execution to script.sh 
        mode_t execute_mode = mode | S_IXUSR | S_IXGRP | S_IXOTH;
        if (chmod(full_path, execute_mode) == -1)
            return -errno;
    }

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
```
Pada kode diatas ada beberapa fungsi yang diberikan yaitu 
1. Logika untuk membuat sebuah direktori baru 
2. Logika untuk merename sebuah direktori 
3. Logika untuk mengatur permisiion atau izin suatu program ataupun file 
4. Logika untuk membuat file baru 
5. Logika untuk menghapus suatu file 


snprintf

snprintf adalah sebuah fungsi dalam bahasa pemrograman C yang digunakan untuk menulis format string ke dalam buffer. Format string tersebut kemudian disalin ke dalam array yang ditentukan oleh parameter str.

1. custom_mkdir(const char *path, mode_t mode)

Fungsi ini digunakan untuk membuat sebuah direktori baru pada path yang ditentukan.
        Fungsi pertama-tama menggunakan snprintf untuk menggabungkan directory_path dan path ke dalam full_path.
        Kemudian, menggunakan mkdir() untuk membuat direktori baru dengan path yang diberikan dan mode permission yang ditentukan.

2. custom_rename(const char *from, const char *to)

Fungsi ini digunakan untuk mengubah nama suatu berkas atau direktori.
        Fungsi menggunakan snprintf untuk menggabungkan directory_path dan from ke dalam from_path, serta directory_path dan to ke dalam to_path.
        Menggunakan rename() untuk mengubah nama dari from menjadi to.
        Setelah proses pengubahan nama selesai, fungsi akan menambahkan watermark pada berkas yang baru diubah namanya menggunakan perintah eksternal system().

3. custom_chmod(const char *path, mode_t mode)

Fungsi ini digunakan untuk mengubah permission dari suatu berkas.
        Fungsi menggunakan snprintf untuk menggabungkan directory_path dan path ke dalam full_path.
        Menggunakan chmod() untuk mengubah permission dari berkas yang ditentukan.
        Jika berkas yang akan diubah permission-nya adalah script.sh, fungsi juga menambahkan permission eksekusi ke dalam permission yang diberikan.

4. custom_create(const char *path, mode_t mode, struct fuse_file_info *fi)

Fungsi ini digunakan untuk membuat berkas baru
        Fungsi menggunakan snprintf untuk menggabungkan directory_path dan path ke dalam full_path.
        Menggunakan creat() untuk membuat berkas baru dengan path dan mode permission yang ditentukan.

5. custom_unlink(const char *path)

Fungsi ini digunakan untuk menghapus sebuah berkas.
        Fungsi menggunakan snprintf untuk menggabungkan directory_path dan path ke dalam full_path.
        Menggunakan unlink() untuk menghapus berkas yang ditentukan.


```c
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
```
Dari kode diatas ada beberapa fungsi utamanya :
1. Melihat sebuah file dengan nama depan test akan secara reverse dengan cara menggunakan buffer yang terdapat pointer i dan j lalu nanti bacanya dibalik 
2. Menulis sebuah file 
3. Mengcopy sebuah file 
4. Memberikan watermark (Baik  di file yang di copy, dipindahkan direktorinya, ataupun diubah namanya) 

Penjelasan dari logika kode diatas adalah 

1. reverse_buffer(char *buf, size_t size)

Fungsi ini bertujuan untuk membalik isi dari sebuah buffer.
        Menggunakan pendekatan dua pointer (i dan j) yang mulai dari awal dan akhir buffer untuk menukar setiap elemen hingga titik pertemuan di tengah buffer.

2. contains_string(const char *haystack, const char *needle)

Fungsi ini digunakan untuk memeriksa apakah sebuah string mengandung string lainnya.
        Menggunakan loop untuk memeriksa setiap substring dari haystack yang panjangnya sama dengan panjang needle. Jika substring tersebut sama dengan needle, maka fungsi mengembalikan 1. Jika tidak ditemukan, mengembalikan 0.

3. custom_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)

Fungsi ini digunakan untuk membaca isi dari sebuah berkas dengan kemungkinan membalik isi berkas jika namanya mengandung kata "test".
        Memeriksa apakah nama berkas mengandung kata "test".
        Jika iya, berkas dibuka untuk dibaca (O_RDONLY), kemudian dibaca ke dalam buffer. Setelah itu, isi buffer dibalik menggunakan fungsi reverse_buffer.
        Jika tidak, berkas dibaca normal tanpa pembalikan isi.
        Mengembalikan jumlah byte yang berhasil dibaca.

4. custom_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)

Fungsi ini digunakan untuk menulis ke sebuah berkas.
        Berfungsi mirip dengan custom_read, namun menggunakan pwrite untuk menulis ke berkas.

5. custom_copy(const char *from, const char *to)

Fungsi ini digunakan untuk menyalin berkas dari satu lokasi ke lokasi lainnya.
        Membuka berkas sumber untuk dibaca (O_RDONLY) dan berkas tujuan untuk ditulis (O_WRONLY | O_CREAT | O_TRUNC).
        Menggunakan loop untuk membaca sebagian dari berkas sumber dan menulisnya ke berkas tujuan.
        Menutup kedua berkas setelah selesai.

6. custom_watermark(const char *path)

Fungsi ini digunakan untuk menambahkan watermark ke sebuah berkas gambar.
        Menggunakan perintah eksternal convert dari ImageMagick untuk menambahkan watermark ke berkas gambar.
        Perintah dieksekusi menggunakan system().

7. custom_mkdir_watermark(const char *path, mode_t mode) & custom_rename_watermark(const char *from, const char *to)

Fungsi-fungsi ini adalah varian dari custom_mkdir dan custom_rename yang menambahkan watermark ke direktori atau berkas yang baru dibuat atau diubah namanya.
        Setelah melakukan operasi custom_mkdir atau custom_rename, fungsi ini memanggil custom_watermark untuk menambahkan watermark ke objek yang baru dibuat atau diubah namanya.

```c
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
```

Dan terakhir pada akhir kode 
 terdapat struktur fuse_operations ini mendefinisikan setiap operasi yang akan diimplementasikan oleh sistem file yang disediakan.

    getattr: Mengambil atribut dari sebuah berkas atau direktori.
    readdir: Membaca isi dari sebuah direktori.
    mkdir: Membuat sebuah direktori baru dengan tambahan watermark.
    rename: Mengubah nama dari sebuah berkas atau direktori dengan tambahan watermark.
    chmod: Mengubah permission dari sebuah berkas. (khususnya script.sh)
    create: Membuat sebuah berkas baru.
    unlink: Menghapus sebuah berkas.
    read: Membaca isi dari sebuah berkas, dengan kemungkinan pembalikan isi jika nama berkas mengandung kata "test".
    write: Menulis ke sebuah berkas.

Setiap operasi dihubungkan dengan fungsi-fungsi yang telah didefinisikan sebelumnya. Sehingga nanti pada terminal hanya menulis chmod, atau create atau mkdir untuk menjalankan logika-logika pada kode. 

Dan pada int main sebagai titik masuk program yang bisa mengmount sebuah direktori kosonng yang akan digunakan sebagai tempat FUSE

 ## ***Dokumentasi***
![awal](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150375098/679d8d70-0eda-448b-94e1-4e4e3912d26d)
![Setelah Mount](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150375098/f176d842-b05f-410b-a222-ca29501be39a)
![Sebelum Watermark](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150375098/006296b0-349f-4469-9c37-9d91554bbb73)
![Setelah Watermark](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150375098/85675b54-790c-416e-b99a-351f2f6dea41)
![File test awal](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150375098/b09cdf1f-ba5f-4653-b066-d74597e09791)
![Langsung ter chmod](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150375098/e1345745-c175-4251-88c4-e6b627afad2c)
![test di fuse](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150375098/e35d4ade-e4d0-45c9-ae1a-9457512ae74e)

## ***SOAL 2 (Abhinaya)***

Masih dengan Ini Karya Kita, sang CEO ingin melakukan tes keamanan pada folder sensitif Ini Karya Kita. Karena Teknologi Informasi merupakan departemen dengan salah satu fokus di Cyber Security, maka dia kembali meminta bantuan mahasiswa Teknologi Informasi angkatan 2023 untuk menguji dan mengatur keamanan pada folder sensitif tersebut. Untuk mendapatkan folder sensitif itu, mahasiswa IT 23 harus kembali mengunjungi website Ini Karya Kita pada `www.inikaryakita.id/schedule` . Silahkan isi semua formnya, tapi pada form subject isi dengan nama kelompok_SISOP24 , ex: **IT01_SISOP24** . Lalu untuk form Masukkan Pesanmu, ketik “**Mau Foldernya**” . Tunggu hingga 1x24 jam, maka folder sensitif tersebut akan dikirimkan melalui email kalian. Apabila folder tidak dikirimkan ke email kalian, maka hubungi sang `CEO` untuk meminta bantuan.

- Pada folder "**pesan**" Adfi ingin meningkatkan kemampuan sistemnya dalam mengelola berkas-berkas teks dengan menggunakan **fuse**.
  + Jika sebuah file memiliki prefix "**base64**," maka sistem akan langsung mendekode isi file tersebut dengan algoritma Base64.
  + Jika sebuah file memiliki prefix "**rot13**," maka isi file tersebut akan langsung di-decode dengan algoritma ROT13.
  + Jika sebuah file memiliki prefix "**hex**," maka isi file tersebut akan langsung di-decode dari representasi heksadesimalnya.
  + Jika sebuah file memiliki prefix "**rev**," maka isi file tersebut akan langsung di-decode dengan cara membalikkan teksnya.
  + Contoh:

    File yang belum didecode/ dienkripsi rot_13
![Cuplikan layar 2024-05-23 194421](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/a10b739f-47d5-4615-aff6-59a506091a9a)

    File yang sudah didecode/ dienkripsi rot_13
![Cuplikan layar 2024-05-23 194429](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/a6513c9f-75ed-4a2e-81f4-fa035ec99f8c)

- Pada folder “**rahasia-berkas**”, Adfi dan timnya memutuskan untuk menerapkan kebijakan khusus. Mereka ingin memastikan bahwa folder dengan prefix "rahasia" tidak dapat diakses tanpa izin khusus. 
  + Jika seseorang ingin mengakses folder dan file pada “rahasia”, mereka harus memasukkan sebuah password terlebih dahulu (password bebas). 
- Setiap proses yang dilakukan akan tercatat pada **logs-fuse.log** dengan format :

**[SUCCESS/FAILED]::dd/mm/yyyy-hh:mm:ss::[tag]::[information]**
```
Ex:
[SUCCESS]::01/11/2023-10:43:43::[moveFile]::[File moved successfully]
```

 ## ***PENGERJAAN***
```c
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
```

 ## ***PENJELASAN PENGERJAAN***
Kode ini adalah implementasi sederhana dari filesystem menggunakan FUSE (Filesystem in Userspace). Filesystem ini memiliki beberapa fitur unik, termasuk pembacaan file yang dienkripsi dengan base64, rot13, hex, dan reverse. Selain itu, filesystem ini juga memiliki mekanisme penguncian akses ke direktori rahasia menggunakan password.

1. Header dan Variabel Global
```c
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

static const char *dirpath = "/home/abhi/sisop/sensitif";
const char *correct_password = "password123";
int entered_pass = 0;
```
Bagian ini mendefinisikan versi FUSE yang digunakan dan mengimpor pustaka-pustaka yang diperlukan. Variabel global dirpath digunakan untuk menentukan path direktori dasar, correct_password menyimpan password yang benar, dan entered_pass adalah flag untuk melacak apakah password sudah dimasukkan dengan benar.

2. Fungsi logging
```c
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
```
Fungsi log_operation mencatat operasi yang dilakukan oleh filesystem ke dalam file log.

3. Fungsi untuk meminta password
```c
int prompt_password() {
    char input_password[256];
    printf("Enter password to access this folder: ");
    if (scanf("%255s", input_password) != 1) {
        return 0;
    }
    return strcmp(input_password, correct_password) == 0;
}
```
Fungsi prompt_password meminta pengguna untuk memasukkan password dan memverifikasinya dengan password yang benar.

4. Fungsi untuk decode Base64
```c
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
```
Fungsi base64_decode mendekode data yang dienkripsi dengan base64.

5. Fungsi untuk decode ROT13
```c
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
```
Fungsi rot13_decode mendekode data yang dienkripsi dengan rot13.

6. Fungsi untuk decode Hex
```c
char *hex_decode(const char *data, size_t *out_len) {
    size_t len = strlen(data);

    *out_len = len / 2;
    char *buffer = malloc(*out_len);

    for (size_t i = 0; i < *out_len; i++) {
        sscanf(data + 2 * i, "%2hhx", &buffer[i]);
    }
    return buffer;
}
```
Fungsi hex_decode mendekode data yang dienkripsi dengan hex.

7. Fungsi untuk Membalik String
```c
void reverse(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}
```
Fungsi reverse membalikkan urutan karakter dalam string.

8. Implementasi Fungsi FUSE
- getattr
```c
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
```
Fungsi ini mengambil atribut file. Jika gagal, operasi dicatat sebagai gagal; jika berhasil, dicatat sebagai sukses.
- readdir
```c
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
```  
Fungsi ini membaca isi direktori dan mengisi buffer dengan nama file. Hasil operasi juga dicatat.
- open
```c
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
```
Fungsi ini membuka file dan memeriksa apakah file berada di direktori rahasia, serta meminta password jika diperlukan.
- read
```c
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
```
Fungsi ini membaca isi file dan mendekode isi file berdasarkan nama file yang memiliki prefiks tertentu (base64, rot13, hex, rev).
- opendir
```c
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
```
Fungsi ini membuka direktori dan memeriksa apakah direktori rahasia diakses, kemudian meminta password jika diperlukan.
- releasedir
```c
static int pesan_releasedir(const char *path, struct fuse_file_info *fi) {
    // Check for "rahasia-berkas" prefix
    if (strncmp(path, "/rahasia-berkas", 15) != 0) {
        entered_pass = 0;
    }
    log_operation("SUCCESS", "releasedir", path);
    return 0;
}
```
Fungsi ini me-reset status password ketika direktori ditutup.

9. Struktur Operasi FUSE
```c
static struct fuse_operations pesan_oper = {
    .getattr    = pesan_getattr,
    .readdir    = pesan_readdir,
    .opendir    = pesan_opendir,
    .open       = pesan_open,
    .read       = pesan_read,
    .releasedir = pesan_releasedir
};
```
Struktur fuse_operations berisi pointer ke fungsi-fungsi yang mengimplementasikan operasi filesystem.

10. Fungsi main
```c
int main(int argc, char *argv[]) {
    return fuse_main(argc, argv, &pesan_oper, NULL);
}
```
Fungsi main adalah titik masuk program yang memanggil fuse_main dengan argumen yang diterima dari command line dan struktur operasi yang telah didefinisikan.
 ## ***Dokumentasi***
![Screenshot from 2024-05-25 17-49-29](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/123524655/08a48421-9c90-47bc-b0e4-72bb9702ebd8)
![Screenshot from 2024-05-25 17-50-21](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/123524655/d410f8bb-6f2c-40e6-92d2-7e12ef637ba4)
![Screenshot from 2024-05-25 17-50-26](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/123524655/0f0abbd1-0268-4d9b-80f2-464d6e2a1c4e)
![Screenshot from 2024-05-25 17-51-04](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/123524655/53ec0a46-e726-4143-80c2-c525be4b2dec)
 ## ***SOAL 3 (Fidel)***



 Seorang arkeolog menemukan sebuah gua yang didalamnya tersimpan banyak relik dari zaman praaksara, sayangnya semua barang yang ada pada gua tersebut memiliki bentuk yang terpecah belah akibat bencana yang tidak diketahui. Sang arkeolog ingin menemukan cara cepat agar ia bisa menggabungkan relik-relik yang terpecah itu, namun karena setiap pecahan relik itu masih memiliki nilai tersendiri, ia memutuskan untuk membuat sebuah file system yang mana saat ia mengakses file system tersebut ia dapat melihat semua relik dalam keadaan utuh, sementara relik yang asli tidak berubah sama sekali.

Ketentuan :

a. Buatlah sebuah direktori dengan ketentuan seperti pada tree berikut
```
.
├── [nama_bebas]
├── relics
│   ├── relic_1.png.000
│   ├── relic_1.png.001
│   ├── dst dst…
│   └── relic_9.png.010
└── report
```
b. Direktori [**nama_bebas**] adalah direktori FUSE dengan direktori asalnya adalah direktori relics. Ketentuan Direktori [**nama_bebas**] adalah sebagai berikut :
   - Ketika dilakukan listing, isi dari direktori [**nama_bebas**] adalah semua relic dari relics yang telah tergabung.
     ![Cuplikan layar 2024-05-23 195252](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/56b17690-64e1-4492-aaa6-2c501e48587f)
   -Ketika dilakukan copy (dari direktori [**nama_bebas**] ke tujuan manapun), file yang disalin adalah file dari direktori **relics** yang sudah tergabung.
    ![Cuplikan layar 2024-05-21 004410](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/951977d7-3300-444d-9b07-ec75e92c219d)
   - Ketika ada file dibuat, maka pada direktori asal (direktori **relics**) file tersebut akan dipecah menjadi sejumlah pecahan dengan ukuran maksimum tiap pecahan adalah 10kb.
     ![Cuplikan layar 2024-05-21 004416](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/ef0eee20-0d53-4ddf-a6c4-071e8bfaad9b)
   - File yang dipecah akan memiliki nama **[namafile].000** dan seterusnya sesuai dengan jumlah pecahannya.
   - Ketika dilakukan penghapusan, maka semua pecahannya juga ikut terhapus.

c. Direktori **report** adalah direktori yang akan dibagikan menggunakan Samba File Server. Setelah kalian berhasil membuat direktori [**nama_bebas**], jalankan FUSE dan salin semua isi direktori [**nama_bebas**] pada direktori **report**.
![Cuplikan layar 2024-05-21 004427](https://github.com/Rafjonath/Sisop-4-2024-MH-IT26/assets/150430084/2ffc6872-eccd-4e35-8681-b97e95cf2ca8)

d. Catatan:
   - pada contoh terdapat 20 relic, namun pada zip file hanya akan ada 10 relic
   - [**nama_bebas**] berarti namanya dibebaskan
   - pada soal 3c, cukup salin secara manual. File Server hanya untuk membuktikan bahwa semua file pada direktori [**nama_bebas**] dapat dibuka dengan baik.

e. discoveries.zip

Changelog:
  - [18/05/2024] memperjelas keterangan soal no. 3, ada tambahan catatan.

 ## ***PENGERJAAN***
```c
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

static const char *relics_path = "/home/ubuntu/sisop4/relics";
static const size_t CHUNK_SIZE = 10240; 

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

static int relic_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    (void) fi;
    char fullpath[1024];
    get_fullpath(fullpath, path, 0);

    strcat(fullpath, ".000");

    int fd = open(fullpath, fi->flags, mode);
    if (fd == -1)
        return -errno;

    close(fd);
    return 0;
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
    .create     = relic_create,
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
```
 ## ***PENJELASAN PENGERJAAN***

1.Library
```c
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
```
+ #define FUSE_USE_VERSION 30: Menentukan versi API FUSE yang digunakan (3.0).
+ #include <fuse.h>: Menyertakan header untuk menggunakan FUSE.
+ #include <stdio.h>, #include <string.h>, #include <errno.h>, #include <fcntl.h>, #include <dirent.h>, #include <stdlib.h>, #include -<sys/types.h>, #include <unistd.h>: Menyertakan header standar C untuk operasi file dan sistem.
```c
static const char *relics_path = "/home/ubuntu/sisop4/relics";
static const size_t CHUNK_SIZE = 10240;
```
+ relics_path: Menentukan direktori dasar untuk file sistem FUSE.
+ CHUNK_SIZE: Menentukan ukuran chunk file (10 KB) untuk pembagian file.

2.Fungsi utility 
```c
static void get_fullpath(char *fullpath, const char *path, int chunk) {
    if (chunk >= 0)
        sprintf(fullpath, "%s%s.%03d", relics_path, path, chunk);
    else
        sprintf(fullpath, "%s%s", relics_path, path);
}
```
 + get_fullpath: Membuat path lengkap untuk file berdasarkan path relatif dan nomor chunk. Jika chunk >= 0, menambahkan nomor chunk ke nama file.

3. Penggunaan Fuse Operations

`#getattr`
```c
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
```
+ relic_getattr: Mengambil atribut file atau direktori. Jika path adalah root ("/"), mengatur atribut direktori. Untuk file, menghitung ukuran total dengan menjumlahkan ukuran semua chunk.

`#readdir`
```c
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
```
+ relic_readdir: Membaca isi direktori. Jika path adalah root, membuka relics_path dan mengisi buffer dengan nama entri direktori. Hanya menampilkan file yang memiliki ekstensi ".000" (chunk pertama).

`#open`
```c
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
```
+ relic_open: Membuka file berdasarkan path. Memastikan file chunk pertama ada.

`#read`
```c
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
```
+ relic_read: Membaca data dari file. Membaca data dari beberapa chunk jika diperlukan, berdasarkan offset dan ukuran yang diminta.

`#write`
```c
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
```
+ relic_write: Menulis data ke file. Menulis data ke beberapa chunk jika diperlukan, berdasarkan offset dan ukuran yang diberikan.

`#create`
```c
static int relic_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    (void) fi;
    char fullpath[1024];
    get_fullpath(fullpath, path, 0);

    strcat(fullpath, ".000");

    int fd = open(fullpath, fi->flags, mode);
    if (fd == -1)
        return -errno;

    close(fd);
    return 0;
}
```
+ relic_create: Membuat file baru. File baru selalu dimulai dengan chunk pertama (".000").

`#unlink`
```c
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
```
+ relic_unlink: Menghapus file. Menghapus semua chunk dari file.

4.Fungsi Main
```c
static struct fuse_operations relic_oper = {
    .getattr    = relic_getattr,
    .readdir    = relic_readdir,
    .open       = relic_open,
    .read       = relic_read,
    .write      = relic_write,
    .create     = relic_create,
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
```
+ struct fuse_operations relic_oper: Mendefinisikan operasi yang diimplementasikan.
+ main: Fungsi utama yang memulai FUSE. Mengecek argumen input untuk mount point, menambahkan opsi "nonempty" ke argumen FUSE, dan memanggil fuse_main untuk memulai FUSE.

#CARA KERJA

+ Mounting: File sistem ini di-mount pada direktori yang ditentukan oleh pengguna.
+ Akses File dan Direktori: Implementasi FUSE operation handle berbagai operasi seperti mendapatkan atribut file (getattr), membaca direktori (readdir), membuka file (open), membaca file (read), menulis file (write), membuat file (create), dan menghapus file (unlink).
+ Chunking: File dipecah menjadi chunk berukuran tetap (10 KB). Setiap chunk disimpan sebagai file terpisah dengan ekstensi berurutan (.000, .001, dst).
+ Penggabungan dan Pemisahan: Saat membaca atau menulis file, sistem menggabungkan atau memisahkan chunk sesuai dengan offset dan ukuran yang diminta.

## ***REVISI***
# Samba config belum dibuat
```
// Samba conf:
// [<>]
//     comment = Report - Soal 4 Modul 3 Sisop
//     path = /home/ubuntu/sisop4/report
//     read only = yes
//     browsable = yes
//     writable = no
//     guest ok = yes
```
#### CARA KERJA

+ Salin file secara manual: cp fuze/. report/, kemudian input buat konfigurasi samba
+ Cari ip address dengan ip addr, kemudian input ke smb:///<>
+ Masuk dengan username ubuntu, password ubuntu

 ## ***Dokumentasi***
 ![Cuplikan layar 2024-05-25 132352](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150430084/5c0b5191-774e-4416-ba36-2b4a0803d8a2)
 ![Cuplikan layar 2024-05-25 132659](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150430084/70885f3e-7f21-4b34-b812-9677fbd567fc)
![Cuplikan layar 2024-05-25 132730](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150430084/8e5502e8-e30f-4b7e-9f27-270bbefa24ba)
![Cuplikan layar 2024-05-25 132757](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150430084/74e40268-a80c-4b0c-afd1-3aa0145f2085)
![Cuplikan layar 2024-05-25 132846](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150430084/cea63f10-4edf-47a7-94cc-d95448e7fdab)
![Cuplikan layar 2024-05-25 132948](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150430084/e43a0a93-2aac-465d-ac72-31ccde3e6b25)
![Cuplikan layar 2024-05-25 135547](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150430084/a833db5e-12c8-4cc5-b19a-4788df8eba30)
![Cuplikan layar 2024-05-25 140738](https://github.com/RafaelJonathanA/Sisop-4-2024-MH-IT26/assets/150430084/46337a76-aeaf-4fa2-95e1-c18c1dfcd648)






