#include "Socket.hpp"
#include <iostream>

int main()
{
    SimpleNet::Socket client_socket;
    client_socket.connect("127.0.0.1", 1111);

    client_socket.send("Hello server");

    auto reply_bytes = client_socket.receive();
    std::string reply(reply_bytes.begin(), reply_bytes.end());

    std::cout << "Server responded: " << reply << "\n";
}
