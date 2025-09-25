#include <unistd.h>   // r, w, c
#include <fcntl.h>    // open
#include <stdlib.h>   // exit
#include <stdio.h>    // perror

int main(int argc, char **argv) {
    if (argc < 3) {
        const char usage[] = "Usage: ./simple-copy <source-file> <destination-file>\n";
        if (write(2, usage, sizeof(usage) - 1) == -1) {
            perror("Error writing usage message");
        }
        exit(1);
    }

    const char *srcPath = argv[1];
    const char *dstPath = argv[2];

    int src = open(srcPath, O_RDONLY);
    if (src == -1) {
        perror("Error opening source file");
        exit(1);
    }

    int dst = open(dstPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst == -1) {
        perror("Error opening destination file");
        close(src);
        exit(1);
    }

    char buf[4096];
    ssize_t bytesRead;

    while ((bytesRead = read(src, buf, sizeof(buf))) > 0) {
        ssize_t totalWritten = 0;
        while (totalWritten < bytesRead) {
            ssize_t bytesWritten = write(dst, buf + totalWritten, bytesRead - totalWritten);
            if (bytesWritten == -1) {
                perror("Error writing to destination file");
                close(src);
                close(dst);
                exit(1);
            }
            totalWritten += bytesWritten;
        }
    }

    if (bytesRead == -1) {
        perror("Error reading source file");
        close(src);
        close(dst);
        exit(1);
    }

    close(src);
    close(dst);
    return 0;
}

