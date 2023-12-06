cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

project(talk)

# make sure we have libpthread
find_package(Threads REQUIRED)
find_library(sodium libsodium sodium REQUIRED)
find_library(toxcore libtoxcore toxcore REQUIRED)
find_library(curses libcurses curses REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

set_target_properties(${CMAKE_PEOJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/include)

# create executable, include libtoxchat and link all dependencies
# add_executable(${CMAKE_PROJECT_NAME} echo_bot.cpp)
# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/include)
#target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC /usr/local/lib)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
        Threads::Threads
        sodium
        toxcore
        curses
        #toxchat
)
