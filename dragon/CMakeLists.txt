cmake_minimum_required(VERSION 3.0)

project(dragon)

# TODO: library location might change
include_directories(/usr/local/include/)
LINK_DIRECTORIES(/usr/local/lib/)


add_executable(MainProject MRAA_PWMDriver.cpp testPWMServoDriver.cpp )

TARGET_LINK_LIBRARIES(dragon mraa)

