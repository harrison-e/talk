# Talk

*Used as a final project for CS3520 @ NU*

Talk is a peer-to-peer messaging client for [Tox](https://github.com/TokTok/c-toxcore/).

For this program to work, [install Tox, and all of its dependencies](https://github.com/TokTok/c-toxcore/blob/master/INSTALL.md#requirements) (most notably, [sodium](https://doc.libsodium.org/installation)).

# How to Build

After installing the necessary dependencies, use the provided [CMakeLists.txt](./CMakeLists.txt) to build with CMake.

Alternatively, build with `g++`:
```
g++ main.cpp -lpthread -lsodium -ltoxcore -o <EXECUTABLE>
```

