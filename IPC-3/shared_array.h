#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <cerrno>
#include <unistd.h>
#include <string>
#include <stdexcept>

class SharedArray 
{
private:
    int* data;
    std::string name;
    std::size_t size;
    int shFd;

public:
    SharedArray(std::string name_, std::size_t size_): name(name_), size(size_) 
    {
        if (size == 0) throw std::invalid_argument("-_-");

        shFd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
        if (shFd == -1) {
            throw std::runtime_error("shm_open failed");
        }

        if (ftruncate(shFd, size * sizeof(int)) == -1) {
            close(shFd);
            throw std::runtime_error("ftruncate failed");
        }

        data = static_cast<int*>(mmap(nullptr, size * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shFd, 0));
        if (data == MAP_FAILED) {
            close(shFd);
            throw std::runtime_error("mmap failed");
        }
    }

    int& operator[](std::size_t i) 
    {
        if (i >= size) throw std::out_of_range("Out Of Range");
        return data[i];
    }

    ~SharedArray() 
    {
        munmap(data, size * sizeof(int));
        close(shFd);
        shm_unlink(name.c_str());
    }

    SharedArray(const SharedArray&) = delete;
    SharedArray& operator=(const SharedArray&) = delete;
};

#endif
