#pragma once
#include <netinet/in.h>

namespace metwo
{
    class tcp_client
    {
    private:
        int sock;
        sockaddr_in address;

    public:
        tcp_client(const char* ip, int port);
        void connect_server();
        void write(void* buf, size_t length);
        void read(void* buf, size_t length);
        ~tcp_client();
    };
}