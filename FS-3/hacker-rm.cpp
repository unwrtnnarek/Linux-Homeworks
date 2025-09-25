#include <unistd.h>   // write, close, unlink, lseek
#include <fcntl.h>    // open, O_WRONLY
#include <stdlib.h>   // exit
#include <stdio.h>    // perror
#include <string.h>   // memset

int main(int argc, char **argv) {
    if (argc < 2) {
        const char msg[] = "Usage: ./hacker-rm <file-to-erase>\n";
        if (write(2, msg, sizeof(msg) - 1) == -1) {
            perror("write error");
            exit(1);
        }
        exit(1);
    }

    const char *filePath = argv[1];

    // open file to write
    int fd = open(filePath, O_WRONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    // file size with lseek
    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == -1) {
        perror("Error seeking file");
        close(fd);
        exit(1);
    }

    // kidayem ukazatel v nachalo
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error seeking file");
        close(fd);
        exit(1);
    }

    // deleting and writting zeros 
    const size_t bufSize = 4096;
    char buf[bufSize];
    memset(buf, '\0', bufSize);

    off_t bytesRemaining = fileSize;
    while (bytesRemaining > 0) {
        ssize_t bytesToWrite = (bytesRemaining < (off_t)bufSize) ? bytesRemaining : bufSize;
        ssize_t written = write(fd, buf, bytesToWrite);
        if (written == -1) {
            perror("Error writing to file");
            close(fd);
            exit(1);
        }
        bytesRemaining -= written;
    }

    close(fd);
	//deleting file
   if (unlink(filePath) == -1) {
        perror("Error deleting file");
        exit(1);
    }

    return 0;
}

