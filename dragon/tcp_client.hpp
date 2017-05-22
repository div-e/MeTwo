#pragma once

namespace metwo
{
    class tcp_client
    {
    private:
        int sock;

    public:
        tcp_client(const char* ip, int port);
        void write(void* buf, size_t length);
        void read(void* buf, size_t length);
        ~tcp_client();
    };
}