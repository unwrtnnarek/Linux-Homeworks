#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

std::vector<std::string> tokenize(const std::string& in, char d) {
    std::vector<std::string> result;
    std::stringstream stream(in);
    std::string part;
    while (std::getline(stream, part, d)) {
        if (!part.empty()) {
            result.push_back(part);
        }
    }
    return result;
}

std::vector<std::string> split_command(const std::string& command, const std::string& separator) {
    std::vector<std::string> parts;
    size_t position = command.find(separator);
    
    if (position == std::string::npos) {
        parts.push_back(command);
    } else {
        parts.push_back(command.substr(0, position));
        parts.push_back(command.substr(position + separator.length()));
    }
    return parts;
}

int execute_command(std::string command, int output_descriptor = 1) {
    int process_id = fork();
    
    if (process_id == 0) {
        // child 
        if (output_descriptor != 1) {
            dup2(output_descriptor, 1);
            close(output_descriptor);
        }
        
        std::vector<std::string> arguments = tokenize(command, ' ');
        std::vector<char*> exec_args;
        
        for (const auto& argument : arguments) {
            exec_args.push_back(const_cast<char*>(argument.c_str()));
        }
        exec_args.push_back(nullptr);
        
        execvp(exec_args[0], exec_args.data());
        
        std::cerr << "Failed to execute: " << command << std::endl;
        exit(EXIT_FAILURE);
        
    } else if (process_id > 0) {
        // prent
        int exit_status;
        if (wait(&exit_status) == -1) {
            std::cerr << "Failed to wait for child: " << command << std::endl;
            exit(EXIT_FAILURE);
        }
        
        if (WIFEXITED(exit_status)) {
            return WEXITSTATUS(exit_status);
        }
        
    } else {
        std::cerr << "Fork failed for: " << command << std::endl;
        exit(EXIT_FAILURE);
    }
    
    return 0;
}

int process_command(std::string& user_command) {
    if (user_command.find("&&") != std::string::npos) {
        std::vector<std::string> commands = split_command(user_command, " && ");
        if (process_command(commands[0]) == 0) {
            process_command(commands[1]);
        }
        
    } else if (user_command.find("||") != std::string::npos) {
        std::vector<std::string> commands = split_command(user_command, " || ");
        if (process_command(commands[0]) != 0) {
            process_command(commands[1]);
        }
        
    } else if (user_command.find(';') != std::string::npos) {
        std::vector<std::string> commands = split_command(user_command, "; ");
        process_command(commands[0]);
        process_command(commands[1]);
        
    } else if (user_command.find(">>") != std::string::npos) {
        std::vector<std::string> parts = split_command(user_command, " >> ");
        int file_descriptor = open(parts[1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        
        if (file_descriptor == -1) {
            std::cerr << "Failed to open file: " << parts[1] << std::endl;
            exit(EXIT_FAILURE);
        }
        
        execute_command(parts[0], file_descriptor);
        close(file_descriptor);
        
    } else if (user_command.find('>') != std::string::npos) {
        std::vector<std::string> parts = split_command(user_command, " > ");
        int file_descriptor = open(parts[1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        
        if (file_descriptor == -1) {
            std::cerr << "Failed to open file: " << parts[1] << std::endl;
            exit(EXIT_FAILURE);
        }
        
        execute_command(parts[0], file_descriptor);
        close(file_descriptor);
        
    } else {
        return execute_command(user_command);
    }
    
    return 0;
}

int main() {
    std::string in;
    
    std::cout << "> ";
    std::getline(std::cin, in);
    
    while (in != "exit") {
        if (!in.empty()) {
            process_command(in);
        }
        
        std::cout << "> ";
        std::getline(std::cin, in);
    }
    
    return 0;
}
