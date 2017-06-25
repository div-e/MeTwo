cmake_minimum_required(VERSION 3.0)

include(ExternalProject)

# build libraries and put them into /usr/local
# TODO: decide if we put them locally & statc build
ExternalProject_Add(
    libmraa 
    GIT_REPOSITORY https://github.com/intel-iot-devkit/mraa
    GIT_TAG v1.7.0
    CMAKE_ARGS -D CMAKE_C_COMPILER=${CMAKE_C_COMPILER} -D CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -D BUILDSWIGNODE=OFF
)

ExternalProject_Add(
    libupm
    GIT_REPOSITORY https://github.com/intel-iot-devkit/upm
    GIT_TAG v1.3.0
    CMAKE_ARGS -D CMAKE_C_COMPILER=${CMAKE_C_COMPILER} -D CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -D BUILDSWIGNODE=OFF -D BUILDSWIGPYTHON=OFF
)

ExternalProject_Add(
    opencv
    GIT_REPOSITORY https://github.com/opencv/opencv
    GIT_TAG v3.2.0
    CMAKE_ARGS -D CMAKE_C_COMPILER=${CMAKE_C_COMPILER} -D CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}  -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local
)