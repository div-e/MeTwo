#include <stdio.h>
#include "controls.h"

int init()
{
    printf("%s\n", "Initializing");
    return 0;
}

void forward()
{
    printf("%s\n", "forward");
}

void backward()
{
    printf("%s\n", "backward");
}

void left()
{
    printf("%s\n", "left");
}

void right()
{
    printf("%s\n", "right");
}

void stop()
{
    printf("%s\n", "stop");
}

void stop_turning()
{
    printf("%s\n", "stop_turning");
}
