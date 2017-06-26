cmake_minimum_required(VERSION 3.0)

project(dragon_test_PWM)

# TODO: library location might change
include_directories(/usr/local/include/)
include_directories(../../)
LINK_DIRECTORIES(/usr/local/lib/)

add_executable(dragon_test_PWM MRAA_PWMDriver.cpp testPWMServoDriver.cpp )

TARGET_LINK_LIBRARIES(dragon mraa)
