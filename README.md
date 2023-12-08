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

To get started, users should first make note of their Tox ID, which is displayed on startup, in yellow. 
Users can change their name with [`setname`](#setname) to change their username from the default `anon`.
This can be used with the [`add`](#add) command by a different user to send a friend request. 
After a one user `add`s another, the other user can see the request with [`requests`](#requests) and accept it with [`accept <request-id>`](#accept), replacing `<request-id>` with the appropriate ID shown in `requests`.

The functionality and usage of all commands is noted below:

### start

`start` provides the user with a quick start guide to get up-and-running using Talk. 
This command takes no arguments.

### help

`help` displays all available commands, with a short description. This command can be used with no arguments, or with the name of a command to get just that command's description.

### myid

`myid` displays a user's Tox ID, which can be passed by another user as the first argument to [`add`](#add) to send a friend request. 
This command takes no arguments.

### add

`add` is used to send friend requests to a friend via their Tox ID.

Usage:
```
add <tox-ID> <optional-request-message>
```

### delete

`delete` is used to delete a friend from a user's friends list via the friend's friend number. 
Friend numbers can be seen using [`phonebook`](#phonebook).

Usage:
```
delete <friend-num>
```

### info

`info` will display information about a friend, specified by friend number. 
If given no arguments, it will display info about the user.

Usage:
```
info <friend-num>
```

### history

`history` will display all messages sent between a user and a friend, specified by friend number. 
**Note: currently, chat history is lost after a Talk session is finished.**

Usage:
```
history <friend-num>
```

### setname

`setname` will change the display name of the user.

Usage:
```
setname <new-name>
```

### setstatus

`setstatus` will change the status message of the user.

Usage:
```
setstatus <new-status>
```

### phonebook

`phonebook` will display a list of all current friends of the user. 
This command takes no arguments.

### requests

`requests` displays a list of all pending friend requests sent to the user. 
They can be accepted or denied using [`accept`](#accept) and [`deny`](#deny), respectively. 
This command takes no arguments.

### accept

`accept` will accept a pending friend request, given the request ID.
All pending requests and their IDs can be seen with [`requests`](#requests).

Usage:
```
accept <request-id>
```

### deny

`deny` will deny a pending friend request, given the request ID.
All pending requests and their IDs can be seen with [`requests`](#requests).

Usage:
```
deny <request-id>
```

### talk

`talk` will begin a conversation with a friend, given their friend number.
Using `talk` changes the input mode from command mode to chat mode, which means that all inputs from the user are read as messages, not commands.
To leave a conversation and return to command mode, enter `/leave`.
All friends and their friend numbers can be seen with [`phonebook`](#phonebook).

Usage:
```
talk <friend-num>
```

### save

`save` will save a user's current name, status, friends, and requests to a save file.
By default, this file is `./savedata.tox`, and is in the same directory as the executable.
This command takes no arguments.

### exit 

`exit` will safely end this Talk session.
This command takes no arguments.


## Dependencies

For this program to work, install Tox (`toxcore`), and all of its dependencies, most notably `sodium`.

This can be done with most package managers (`toxcore` and `sodium` have packages available on apt, brew, etc.), or by locally building and installing ([Tox](https://github.com/TokTok/c-toxcore/blob/master/INSTALL.md#requirements) and [sodium]](https://doc.libsodium.org/installation))

For UI, also make sure you have `ncurses` installed (comes preinstalled on most *nix machines).
