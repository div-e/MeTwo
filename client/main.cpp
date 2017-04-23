#include <iostream>
#include "robot.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please pass in the server adddress\n";
        return EXIT_FAILURE;
    }

    metwo::robot robot(argv[1]);
    robot.run();
    std::cout << "Reached the end of the program\n";
    return EXIT_SUCCESS;
}