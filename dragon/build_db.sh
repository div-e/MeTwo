#!/bin/bash

g++ -std=c++11 -g -pthread \
     main.cpp robot.cpp controls_db_pwm.cpp tcp_client.cpp MRAA_PWMDriver.cpp \
     -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lmraa
