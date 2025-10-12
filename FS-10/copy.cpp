#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <source> <destination>\n";
        return EXIT_FAILURE;
    }

    const char* srcPath = argv[1];
    const char* dstPath = argv[2];

    int srcFd = open(srcPath, O_RDONLY);
    if (srcFd < 0) {
        std::perror("Error opening source file");
        return EXIT_FAILURE;
    }

    int dstFd = open(dstPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dstFd < 0) {
        std::perror("Error opening destination file");
        close(srcFd);
        return EXIT_FAILURE;
    }

    constexpr size_t BUF_SIZE = 4096;
    char buffer[BUF_SIZE];
    ssize_t bytesRead;
    off_t totalBytes = 0;
    off_t totalData = 0;
    off_t totalHole = 0;

    while ((bytesRead = read(srcFd, buffer, BUF_SIZE)) > 0) {
        
        bool isHole = true;
        for (ssize_t i = 0; i < bytesRead; ++i) {
            if (buffer[i] != '\0') {
                isHole = false;
                break;
            }
        }

        if (isHole) {

            off_t res = lseek(dstFd, bytesRead, SEEK_CUR);
            if (res == -1) {
                std::perror("Error seeking in destination file");
                close(srcFd);
                close(dstFd);
                return EXIT_FAILURE;
            }
            totalHole += bytesRead;
        } else {
            ssize_t bytesWritten = write(dstFd, buffer, bytesRead);
            if (bytesWritten != bytesRead) {
                std::perror("Error writing to destination file");
                close(srcFd);
                close(dstFd);
                return EXIT_FAILURE;
            }
            totalData += bytesWritten;
        }

        totalBytes += bytesRead;
    }

    if (bytesRead < 0) {
        std::perror("Error reading source file");
        close(srcFd);
        close(dstFd);
        return EXIT_FAILURE;
    }

    close(srcFd);
    close(dstFd);

    std::cout << "Successfully copied " << totalBytes
              << " bytes (data: " << totalData
              << ", hole: " << totalHole << ").\n";

    return EXIT_SUCCESS;
}

