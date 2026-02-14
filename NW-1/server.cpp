#include "TcpServer.hpp"
#include "Socket.hpp"
#include <iostream>

int main()
{
    SimpleNet::TcpServer server(1111);

    server.run([](SimpleNet::Socket client_socket)
    {
        auto request_bytes = client_socket.receive();
        std::string request(request_bytes.begin(), request_bytes.end());

        std::cout << "Received: " << request << "\n";
        client_socket.send("Echo: " + request);
    });
}
