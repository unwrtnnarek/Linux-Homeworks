#include <unistd.h>   // write, read, close
#include <fcntl.h>    //  open
#include <stdlib.h>   // exit
#include <stdio.h>    // perror

int main(int argc, char **argv) {
    const char msg[] = "Usage: ./read-file <filename>\n";
    const char err[] = "Error while reading file\n";

    if (argc < 2) {
        if (write(2, msg, sizeof(msg) - 1) == -1) {
            perror("write");
        }
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        if (write(2, err, sizeof(err) - 1) == -1) {
            perror("write");
        }
        exit(1);
    }

    char buf[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buf, sizeof(buf))) > 0) {
        ssize_t written = write(1, buf, bytesRead);
        if (written == -1) {
            perror("write");
            close(fd);
            exit(1);
        }
    }

    if (bytesRead == -1) {
        perror("read");
        close(fd);
        exit(1);
    }

    close(fd);
    return 0;
}
		   
