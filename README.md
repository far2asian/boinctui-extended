# boinctui-extended
Original code by Sergey Suslov (suleman1971@gmail.com)  
Improved build by Mark Pentler (https://github.com/mpentler/)  

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
- Code comments are being translated into English to assist in working with the codebase
- Typos and some UI strings corrected
- & other code improvements (cppcheck and build environment stuff)

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
After starting the application you can do a few things:
- Connect to an account manager in the project menu
- Attach to projects manually
- Add other BOINC hosts to control
- Configure displayed columns and sort order
- Configure BOINC network and run modes

All of these options are in the menus (M key or F9) and are self-explanatory.

## Useful hot keys
- M/F9 - Open menus
- N - Show next host in the list
- C - Configure host list
- V - View statistics
- Arrow keys - Navigate task list and menus (mouse also supported!)
- S - Suspend selected task
- R - Resume (or queue) selected task
- A - Abort selected task (requires confirmation)
- Enter - View extended info for a task
- Q - Quit boinctui (hit Escape to close windows first)

## Issues
Nothing specific, but I've had a few segmentation faults with the original codebase so it needs a good pruning by someone who actually knows C++ - maybe that's you!?

## Acknowledgments
Additions to the original codebase are from the following GitHub users:
- DerVerruckteFuchs - transparent backgrounds
- zeekec - improved time display, menu hot key, and some code & build improvements
- vorot93 - code improvements
