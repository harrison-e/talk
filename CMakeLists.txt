cmake_minimum_required(VERSION 3.26)
project(tuitalk)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_link_libraries(tuitalk
        Threads::Threads
        ${CMAKE_SOURCE_DIR}/lib/libtoxcore.a)