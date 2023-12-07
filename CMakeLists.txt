cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

project(talk)

# Find required libraries
find_package(Threads REQUIRED)

# Set library paths based on the platform
if(APPLE)
    find_library(SODIUM_LIBRARY
        NAMES sodium
        HINTS /opt/homebrew/lib /usr/local/lib
        REQUIRED
    )
    find_library(TOX_LIBRARY
        NAMES toxcore
        HINTS /opt/homebrew/lib /usr/local/lib
        REQUIRED
    )
    find_library(CURSES
        NAMES curses libcurses ncurses libncurses
        HINTS /usr/lib /lib
        REQUIRED
    )
    # Include Tox header files from Homebrew
    find_path(TOX_INCLUDE_DIR tox/tox.h HINTS /opt/homebrew/include /usr/local/include)
elseif(UNIX)
    find_library(SODIUM_LIBRARY
        NAMES sodium
        HINTS /usr/local/lib
        REQUIRED
    )
    find_library(TOX_LIBRARY
        NAMES toxcore
        HINTS /usr/local/lib
        REQUIRED
    )
    find_library(CURSES
        NAMES curses libcurses ncurses libncurses
        HINTS /usr/lib /usr/include
        REQUIRED
    )
    # Include Tox header files (modify this according to the actual location)
    set(TOX_INCLUDE_DIR "/usr/local/include")
endif()

# Create the executable
add_executable(${PROJECT_NAME} main.cpp
        lib/include/Message.h)

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PRIVATE lib/include ${TOX_INCLUDE_DIR})

# Link libraries to the executable
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Threads::Threads
    ${SODIUM_LIBRARY}
    ${TOX_LIBRARY}
    ${CURSES}
)
