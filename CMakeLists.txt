cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)

project(talk)

# make sure we have libpthread
find_package(Threads REQUIRED)

# create a statically linked lib called libtoxchat
add_library(toxchat
        lib/include/Friend.h
        lib/src/Friend.cpp
        lib/include/Session.h
        lib/src/Session.cpp
        lib/include/Utility.h
)
target_include_directories(toxchat PRIVATE lib/include)

# create executable, include libtoxchat and link all dependencies
# add_executable(${CMAKE_PROJECT_NAME} echo_bot.cpp)
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/include)
target_link_libraries(talk PRIVATE
        Threads::Threads
        sodium
        toxcore
        toxchat
)