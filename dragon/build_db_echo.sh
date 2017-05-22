#!/bin/bash

g++ -std=c++11 -pthread \
     main.cpp robot.cpp echo.cpp tcp_client.cpp -lopencv_core \
    -lopencv_videoio -lopencv_imgcodecs