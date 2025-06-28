// 1- first create the file 
// 2- get the file descriptor
// 3- get the stat of the file
// 4- check the file size
// 5- write to the file
// 6- check the file size again
// 8- go to step 6 and repeat until the file size is greater than 1 MB
// 9- close the file descriptor
// 10- delete the file

#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

void write_to_file_one_byte(int fd) {
    const char data = 'H';
    ssize_t bytes_written = write(fd, &data, 1);
    if (bytes_written == -1) {
        perror("write failed");
        exit(1);
    }


}

void get_file_stat(int fd, struct stat *sb, size_t *size) {
    if (fstat(fd, sb) == -1) {
        perror("fstat failed");
        exit(1);
    }
    *size = sb->st_size;
    printf("File size: %ld bytes\n", *size);
    while (*size < 1024 * 1024) {
        for (int i = 0; i < 1024; i++) { // Write 1 KB in 1024 iterations
            write_to_file_one_byte(fd);
        }

        if (fstat(fd, sb) == -1) {
            perror("fstat failed during loop");
            exit(1);
        }

        if (sb->st_size != *size) {
            *size = sb->st_size;
            printf("File size: %ld bytes\n", *size);
        }
    }
    printf("File size is greater than 1 MB, stopping write.\n");
}


int main(int argc, char *argv[]) {
    struct stat sb;
    size_t size = 0;
    
    int fd = creat(argv[1], 0644);
    if (fd == -1) {
        printf("File creation failed\n");
        exit(1);
    }
    printf("File created successfully with file descriptor: %d\n", fd);
    get_file_stat(fd, &sb, &size);
    close(fd);
    
 }

