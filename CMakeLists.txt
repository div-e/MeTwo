cmake_minimum_required(VERSION 3.0)

include(ExternalProject)

# TODO: maybe static build the library?
ExternalProject_Add(
    mraa 
    GIT_REPOSITORY https://github.com/intel-iot-devkit/mraa
    GIT_TAG v1.7.0
    CMAKE_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
)

include(${CMAKE_SOURCE_DIR}/dragon/CMakeLists.txt)
