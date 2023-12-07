<div style="display:flex">
    <div style="flex:40%">
        <img src="screen_linux_1.png" style="width:100%">
    </div>
    <div style="flex:40%">
        <img src="screen_mac.png" style="width:100%">
    </div>
</div>

<p display="flex"> 
    <img src="screen_linux_1.png" flex=45%>
    <img src="screen_mac.png" flex=45%>
</p>


| ![](screen_linux_1) | ![](screen_mac) |
|---|---|
| talk on Linux | or talk on Mac |

# Talk

*Used as a final project for CS3520 @ NU*

Talk is an encrypted, peer-to-peer messaging client for [Tox](https://github.com/TokTok/c-toxcore/).


# Dependencies

For this program to work, [install Tox, and all of its dependencies](https://github.com/TokTok/c-toxcore/blob/master/INSTALL.md#requirements) (most notably, [sodium](https://doc.libsodium.org/installation)).

For UI, also make sure you have `ncurses` installed (comes preinstalled on most *nix machines).


# How to Build

After installing the necessary dependencies, use the provided [CMakeLists.txt](./CMakeLists.txt) to build with CMake.
```
mkdir build
cd build
cmake ..
```

Alternatively, build with `g++`:
```
g++ -std=c++17 lib/include/* main.cpp -lpthread -lsodium -ltoxcore -lcurses -o talk
```

Then, run with `./talk`
