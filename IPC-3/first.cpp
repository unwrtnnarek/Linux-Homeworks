#include "shared_array.h"
#include <semaphore.h>

int main() 
{
    int arrSize = 10;
    SharedArray arr("shArr", arrSize);

    sem_t* write = sem_open("write", O_CREAT, 0666, 1);
    sem_t* read = sem_open("read", O_CREAT, 0666, 0);

    if (write == SEM_FAILED || read == SEM_FAILED) 
    {
        perror("semaphore open error!");
        exit(errno);
    }

    while (true) 
    {
        sem_wait(write);

        for (int i = 0; i < arrSize; ++i)
            arr[i] = rand() % 100;

        std::cout << "First Program" << std::endl;

        sem_post(read);
        sleep(1);
    }

    sem_close(write);
    sem_close(read);

    return 0;
}
