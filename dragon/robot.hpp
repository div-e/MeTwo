#pragma once
#include <memory>
#include "tcp_client.hpp"

namespace metwo
{
    class robot
    {
    private:
        char* serverAddr;
        static void webcam_thread(std::shared_ptr<tcp_client>);
        static void control_thread(std::shared_ptr<tcp_client>);

    public:
        robot(char* ip);
        void run();
    };
}