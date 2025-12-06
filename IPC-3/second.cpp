#include "shared_array.h"
#include <semaphore.h>

int main() 
{
    int arrSize = 10;
    SharedArray arr("shArr", arrSize);

    sem_t* write = sem_open("write", 0);
    sem_t* read = sem_open("read", 0);

    if (write == SEM_FAILED || read == SEM_FAILED) 
    {

        perror("semaphore open error!");
        exit(errno);
    }

    while (true) 
    {
        sem_wait(read);

        std::cout << "Second Program";
        for (int i = 0; i < arrSize; ++i)
            std::cout << " " << arr[i];
        std::cout << std::endl;

        sem_post(write);
        sleep(1);
    }

    sem_close(writeSem);
    sem_close(readSem);

    return 0;
}
