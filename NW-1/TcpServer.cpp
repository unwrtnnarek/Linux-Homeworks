#include "TcpServer.hpp"

namespace SimpleNet
{

TcpServer::TcpServer(int port, size_t thread_count)
    : listen_socket_(), thread_pool_(thread_count)
{
    listen_socket_.bind(port);
    listen_socket_.listen();
}

void TcpServer::run(ClientHandler handler)
{
    while (true)
    {
        Socket client_socket = listen_socket_.accept();
        auto client_socket_ptr = std::make_shared<Socket>(std::move(client_socket));

        thread_pool_.enqueue([client_socket_ptr, handler]()
        {
            handler(std::move(*client_socket_ptr));
        });
    }
}

} // namespace SimpleNet
