#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

#include "TcpClient.hpp"

MeTwo::TcpClient::TcpClient(int ip1, int ip2, int ip3, int p4, int port)
{
    sock = socket(PF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl((((((ip1 << 8) | ip2) << 8) | ip3) << 8) | ip4);
}

bool MeTwo::TcpClient::Connect() const
{
    if (connect(sock, (sockaddr*) &address, sizeof(address)) < 0)
    {
        return false;
    }
    return true;
}

void MeTwo::TcpClient::Write(void* ptr, size_t size) const
{
    write(sock, ptr, size);
}