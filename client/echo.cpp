#include <iostream>
#include <stdexcept>
#include "controls.hpp"

using namespace std;

void init()
{
    cout << "Initializing\n";
    //throw runtime_error("Failed to initialize");
}

void forward()
{
    cout << "forward\n";
}

void backward()
{
    cout << "backward\n";
}

void left()
{
    cout << "left\n";
}

void right()
{
    cout << "right\n";
}

void stop()
{
    cout << "stop\n";
}

void stop_turning()
{
    cout << "stop_turning\n";
}

void up()
{
    cout << "up\n";
}

void down()
{
    cout << "down\n";
}