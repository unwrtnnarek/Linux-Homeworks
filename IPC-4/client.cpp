#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define CHAT_PORT 12345
#define BUFFER_SIZE 1024

int clientSocket;

void* receiveLoop(void*)
{
    char buffer[BUFFER_SIZE];

    while (true)
    {
        int received = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (received <= 0) break;

        buffer[received] = '\0';
        std::cout << buffer;
    }

    std::cout << "server disconnected.\n";
    exit(0);
}

int main()
{
    sockaddr_in serverAddress{};
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        perror("socket");
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(CHAT_PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("connect");
        return 1;
    }

    std::string username;
    std::cout << "enter username: ";
    std::getline(std::cin, username);
    send(clientSocket, username.c_str(), username.size(), 0);

    pthread_t receiverThread;
    pthread_create(&receiverThread, nullptr, receiveLoop, nullptr);

    std::string outgoing;
    while (true)
    {
        std::getline(std::cin, outgoing);
        send(clientSocket, outgoing.c_str(), outgoing.size(), 0);

        if (outgoing.rfind("/exit", 0) == 0)
            break;
    }

    close(clientSocket);
    return 0;
}
