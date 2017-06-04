#include <iostream>
#include <stdexcept>
#include "controls.hpp"

using namespace std;

void metwo::init()
{
    cout << "Initializing\n";
    //throw runtime_error("Failed to initialize");
}

void metwo::forward()
{
    cout << "forward\n";
}

void metwo::backward()
{
    cout << "backward\n";
}

void metwo::left()
{
    cout << "left\n";
}

void metwo::right()
{
    cout << "right\n";
}

void metwo::stop()
{
    cout << "stop\n";
}

void metwo::stop_turning()
{
    cout << "stop_turning\n";
}

void metwo::up()
{
    cout << "up\n";
}

void metwo::down()
{
    cout << "down\n";
}

void metwo::panRight() 
{
    cout << "panRight\n";
}

void metwo::panLeft() 
{
    cout << "panLeft\n";
}
