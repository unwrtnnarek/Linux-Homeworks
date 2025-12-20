#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define CHAT_PORT 12345
#define BUFFER_SIZE 1024
#define USERNAME_LEN 32

struct User
{
    int socketFd;
    std::string username;
};

std::vector<User*> activeUsers;
pthread_mutex_t usersMutex = PTHREAD_MUTEX_INITIALIZER;

void sendToAll(const std::string& text, int senderFd)
{
    pthread_mutex_lock(&usersMutex);
    for (auto& user : activeUsers)
    {
        if (user->socketFd != senderFd)
        {
            send(user->socketFd, text.c_str(), text.size(), 0);
        }
    }
    pthread_mutex_unlock(&usersMutex);
}

void registerUser(User* user)
{
    pthread_mutex_lock(&usersMutex);
    activeUsers.push_back(user);
    pthread_mutex_unlock(&usersMutex);
}

void unregisterUser(int fd)
{
    pthread_mutex_lock(&usersMutex);
    for (auto it = activeUsers.begin(); it != activeUsers.end(); ++it)
    {
        if ((*it)->socketFd == fd)
        {
            delete *it;
            activeUsers.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&usersMutex);
}

void* clientWorker(void* arg)
{
    User* user = (User*)arg;
    char buffer[BUFFER_SIZE];

    std::string notice = user->username + " joined the chat.\n";
    sendToAll(notice, user->socketFd);

    while (true)
    {
        int bytes = recv(user->socketFd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';
        std::string input(buffer);

        if (input.rfind("/exit", 0) == 0)
        {
            break;
        }
        else if (input.rfind("/list", 0) == 0)
        {
            std::string list = "online users:\n";
            pthread_mutex_lock(&usersMutex);
            for (auto& u : activeUsers)
            {
                list += u->username + "\n";
            }
            pthread_mutex_unlock(&usersMutex);

            send(user->socketFd, list.c_str(), list.size(), 0);
        }
        else
        {
            std::string message = user->username + ": " + input;
            sendToAll(message, user->socketFd);
        }
    }

    notice = user->username + " left the chat.\n";
    sendToAll(notice, user->socketFd);

    close(user->socketFd);
    unregisterUser(user->socketFd);
    pthread_exit(nullptr);
}

int main()
{
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0)
    {
        perror("socket");
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(CHAT_PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("bind");
        return 1;
    }

    if (listen(listenSocket, 10) < 0)
    {
        perror("listen");
        return 1;
    }

    std::cout << "server running on port " << CHAT_PORT << std::endl;

    while (true)
    {
        sockaddr_in clientAddress{};
        socklen_t addressSize = sizeof(clientAddress);

        int clientFd = accept(listenSocket, (sockaddr*)&clientAddress, &addressSize);
        if (clientFd < 0)
        {
            perror("accept");
            continue;
        }

        char nameBuffer[USERNAME_LEN]{};
        recv(clientFd, nameBuffer, USERNAME_LEN - 1, 0);
        nameBuffer[strcspn(nameBuffer, "\n")] = '\0';

        User* user = new User{clientFd, std::string(nameBuffer)};
        registerUser(user);

        pthread_t threadId;
        pthread_create(&threadId, nullptr, clientWorker, (void*)user);
        pthread_detach(threadId);
    }

    close(listenSocket);
    return 0;
}
