<p align="center"> 
    <img src="screen_linux_1.png" width="45%" />
    &nbsp;&nbsp;
    <img src="screen_mac.png" width="45%" />
</p>


# Talk

*Used as a final project for CS3520 @ NU*

**Note to Northeastern students: At present, Talk is unable to connect to users that are connected to NUWave. Apologies for the inconvenience.**

Talk is an encrypted, peer-to-peer messaging client for the C [Tox](https://github.com/TokTok/c-toxcore/) library.
Talk works by connecting users via their Tox ID, allowing for seamless, encrypted, one-on-one messaging.
Talk also leverages curses to create a terminal UI.

By default, no data in Talk persists after the program is closed, unless the user issues the `save` command.
This can be changed by setting the `AUTOSAVE` flag in [main](main.cpp) to `true`.


## Dependencies

For this program to work, install Tox (`toxcore`), and all of its dependencies, most notably `sodium`.

This can be done with most package managers (`toxcore` and `sodium` have packages available on apt, brew, etc.), or by locally building and installing ([Tox](https://github.com/TokTok/c-toxcore/blob/master/INSTALL.md#requirements) and [sodium]](https://doc.libsodium.org/installation))

For UI, also make sure you have `ncurses` installed (comes preinstalled on most *nix machines).


## How to Build

After installing the necessary [dependencies](#Dependencies), use the provided [CMakeLists.txt](./CMakeLists.txt) to build with CMake:
```
mkdir build
cd build
cmake ..
```

Alternatively, build with `g++`:
```
g++ -std=c++17 lib/include/* main.cpp -lpthread -lsodium -ltoxcore -lcurses -o talk
```

Then, run the executable `talk`:
```
./talk
```


## 


