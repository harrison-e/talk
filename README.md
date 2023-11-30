# TUI Talk

*used as a final project for CS3520 @ NU*

TUI Talk is a peer-to-peer messaging client with a TUI.

For network communication, this program implements Tox, a cryptographic network library. For this program to work, [install all dependencies of Tox](https://github.com/TokTok/c-toxcore/blob/master/INSTALL.md#requirements) (mainly, [sodium](https://doc.libsodium.org/installation)).

# How to Build

After installing the necessary dependencies, compile with `g++` as follows:
```
g++ main.cpp lib/libtoxcore.a -lpthread -lsodium -o <EXECUTABLE>
```
