#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include "tcp_client.hpp"

using namespace std;

metwo::tcp_client::tcp_client(const char* ip, int port)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        throw runtime_error("Failed to open socket");
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &address.sin_addr) != 1)
    {
        throw runtime_error("Invaid IP");
    }

    if (connect(sock, (sockaddr*) &address, sizeof(address)) == -1)
    {
        throw runtime_error("Failed to connect");
    }
}

void metwo::tcp_client::write(void* ptr, size_t size)
{
    if (send(sock, ptr, size, 0) == -1)
    {
        throw runtime_error("Failed to send data");
    }
}

void metwo::tcp_client::read(void* buf, size_t length)
{
    if (recv(sock, buf, length, 0) == -1)
    {
        throw runtime_error("Failed to receive data");
    }
}

metwo::tcp_client::~tcp_client()
{
    close(sock);
}