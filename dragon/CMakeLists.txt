cmake_minimum_required(VERSION 3.0)

project(dragon)

# TODO: library location might change
include_directories(/usr/local/include/)
LINK_DIRECTORIES(/usr/local/lib/)

# dependency
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package( Threads REQUIRED )

# include files
file(GLOB C_SOURCES *.c)
file(GLOB CXX_SOURCES *.cpp)

# compile flags
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# compile
add_executable(dragon main.cpp robot.cpp controls.cpp tcp_client.cpp MRAA_PWMDriver.cpp ${C_SOURCES})

# link
TARGET_LINK_LIBRARIES(dragon mraa ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

