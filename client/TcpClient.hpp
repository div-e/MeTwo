#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

namespace MeTwo
{
    class TcpClient
    {
    private:
        int sock;
        sockaddr_in address;

    public:
        TcpClient(int ip1, int ip2, int ip3, int ip4, int port);
        bool Connect() const;
        void Write(void* buf, size_t length) const;
    };
}