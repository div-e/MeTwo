#include <stdio.h>
#include <arpa/inet.h>
#include "controls.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("%s\n", "Please pass in the server adddress");
        return 0;
    }

    if (init())
    {
        printf("%s\n", "Failed to initialize");
        return 0;
    }
    printf("%s\n", "Initialized");

    void (*controls[6])(void) = { forward, backward, stop, left, right, stop_turning };

    printf("%s\n", "Connecting...");
    int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1)
    {
        printf("%s\n", "Error opening socket");
        return 0;
    }
    else
    {
        printf("Socket file descriptor: %d\n", fd);
    }

    struct sockaddr_in ip;
    ip.sin_family = AF_INET;
    ip.sin_port = htons(6000);
    if (!inet_pton(AF_INET, argv[1], &ip.sin_addr))
    {
        printf("%s\n", "Invalid IP");
        return 0;
    }

    if (connect(fd, (struct sockaddr*)&ip, sizeof(ip)) == -1)
    {
        printf("%s\n", "Failed to connect");
        return 0;
    }
    else
    {
        printf("%s\n", "Connected");
    }

    char buffer[1];
    int count;
    while (1)
    {
        count = recv(fd, &buffer, 1, 0);
        if (count == 0)
        {
             printf("%s\n", "Unable to receive instruction");
             break;
        }
        controls[buffer[0] - 1]();
    }
    return 0;
}
