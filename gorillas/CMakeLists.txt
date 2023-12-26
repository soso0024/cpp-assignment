cmake_minimum_required(VERSION 3.1)
project(gorillas CXX)

SET(DUELS_ROOT "/opt/duels" CACHE STRING "Path to duels installation folder")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# find ZeroMQ and yaml-cpp for message passing
find_path ( ZMQ_INCLUDE_DIR zmq.h )
find_library ( ZMQ_LIBRARY NAMES zmq )
find_package(yaml-cpp)

include_directories(${DUELS_ROOT}/include ${ZMQ_INCLUDE_DIR} ${YamlCpp_INCLUDE_DIRS})
add_definitions(-DDUELS_BIN_PATH="${DUELS_ROOT}/bin")

# add whatever additional includes / libs needed for your AI

add_executable(gorillas gorillas.cpp)
target_link_libraries(gorillas ${ZMQ_LIBRARY} yaml-cpp)