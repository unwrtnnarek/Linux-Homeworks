#include "Socket.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <string>

namespace SimpleNet
{

Socket::Socket()
{
    socket_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_ == -1)
    {
        throw std::runtime_error("Failed to create socket");
    }
}

Socket::~Socket()
{
    if (socket_fd_ >= 0)
    {
        ::close(socket_fd_);
    }
}

Socket::Socket(Socket&& other) noexcept : socket_fd_{other.socket_fd_}
{
    other.socket_fd_ = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept
{
    if (this != &other)
    {
        if (socket_fd_ != -1) ::close(socket_fd_);
        socket_fd_ = other.socket_fd_;
        other.socket_fd_ = -1;
    }
    return *this;
}

void Socket::bind(int port)
{
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (::bind(socket_fd_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
    {
        throw std::runtime_error("Failed to bind to port " + std::to_string(port));
    }
}

void Socket::listen(int backlog)
{
    if (::listen(socket_fd_, backlog) == -1)
    {
        throw std::runtime_error("Failed to listen on socket");
    }
}

void Socket::connect(const std::string& ip, int port)
{
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0)
    {
        throw std::runtime_error("Invalid IP address: " + ip);
    }

    if (::connect(socket_fd_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0)
    {
        throw std::runtime_error("Failed to connect to " + ip + ":" + std::to_string(port));
    }
}

Socket Socket::accept()
{
    int client_socket_fd = ::accept(socket_fd_, nullptr, nullptr);
    if (client_socket_fd < 0)
    {
        throw std::runtime_error("Failed to accept connection");
    }
    return Socket(client_socket_fd);
}

std::vector<char> Socket::receive(size_t max_size)
{
    std::vector<char> buffer(max_size);

    ssize_t bytes_received = ::recv(socket_fd_, buffer.data(), buffer.size(), 0);
    if (bytes_received == -1)
    {
        throw std::runtime_error("Failed to recieve data");
    }

    buffer.resize(static_cast<size_t>(bytes_received));
    return buffer;
}

ssize_t Socket::send(std::string_view data)
{
    ssize_t bytes_sent = ::send(socket_fd_, data.data(), data.size(), 0);
    if (bytes_sent == -1)
    {
        throw std::runtime_error("Failed to send data");
    }
    return bytes_sent;
}

} // namespace SimpleNet
