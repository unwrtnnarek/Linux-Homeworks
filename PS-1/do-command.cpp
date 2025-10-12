#include <iostream>
#include <unistd.h>      
#include <sys/wait.h>   
#include <chrono>   

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command> args" << std::endl;
        return 1;
    }

    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        execvp(argv[1], &argv[1]);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(end - start).count();

        if (WIFEXITED(status)) {
            std::cout << "\nCommand completed with "
                      << WEXITSTATUS(status)
                      << " exit code and took "
                      << duration << " seconds."
                      << std::endl;
        } else {
            std::cout << "\nCommand terminated abnormally." << std::endl;
        }
    }

    return 0;
}
