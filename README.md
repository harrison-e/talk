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


## How to Build

After installing the necessary [dependencies](##Dependencies), use the provided [CMakeLists.txt](./CMakeLists.txt) to build with CMake:
```
mkdir build
cd build
cmake ..
```

Alternatively, build on Linux with `g++`:
```
g++ -std=c++17 lib/include/* main.cpp -lpthread -lsodium -ltoxcore -lcurses -o talk
```

Finally, run the executable `talk`:
```
./talk
```


## How to Use

Talk has a set of commands, that allow the user to perform various functions of the Tox API. The user's current input can be seen at the bottom of the screen, after the `>` marker.

To get started, users should first make note of their Tox ID, which is displayed on startup, in yellow. This can be used with the [`add`](#`add`) command by a different user to send a friend request. After a one user `add`s another, the other user can see the request and


### `add`


### `delete`




## Dependencies

For this program to work, install Tox (`toxcore`), and all of its dependencies, most notably `sodium`.

This can be done with most package managers (`toxcore` and `sodium` have packages available on apt, brew, etc.), or by locally building and installing ([Tox](https://github.com/TokTok/c-toxcore/blob/master/INSTALL.md#requirements) and [sodium]](https://doc.libsodium.org/installation))

For UI, also make sure you have `ncurses` installed (comes preinstalled on most *nix machines).
