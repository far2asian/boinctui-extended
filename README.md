# boinctui
Original code by Sergey Suslov (suleman1971@gmail.com)  
Improved build by Mark Pentler (https://github.com/mpentler/)  
- Incorporates various community changes from around GitHub (full list at the bottom)
- Documentation added

## Introduction

boinctui is an curses-based terminal BOINC client manager for Linux. Features include:
- Fullscreen curses-based text user interface
- Switch between several hosts running the BOINC client via a hot key
- View a host's task list (including each task's state)
- View the BOINC log file in a separate window
- Suspend/Resume/Abort control of all tasks
- Update/Suspend/Resume/Reset/No New Task/Allow New Task for attached projects
- Toggle activity state (Always suspend/Always resume/Run based on preferences)
- Manage BOINC client on remote hosts via boinc_gui RPC protocol

Additionally other changes from around GitHub have been merged into this fork:
- Transparent backgrounds supported
- Added another hot key to bring up the menu in case of double-assignment
- Increased precision of time displays
- & other code improvements

## Compiling

Dead easy, but you'll need to install a few libraries first. Here's some instructions 
for Raspbian, which is what I'm doing this on. I have no idea if the package names are 
different on other distributions.

```
sudo apt-get install libssl-dev libexpat1-dev libncursesw5-dev
```

Then to compile:

```
autoconf
./configure
make
```

Finally, to install:

```
sudo make install
```

## Configuration



## Operation



## Issues

Nothing specific, but I've had a few segmentation faults with the original codebase 
so it needs a good pruning by someone who actually knows C++ - maybe that's you!?

## Acknowledgments

Additions to the original codebase are from the following GitHub users:
- DerVerruckteFuchs - transparent backgrounds
- zeekec - improved time display, menu hot key, and some code & build improvements
- vorot93 - code improvements
