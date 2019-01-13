# Neutron-OS

Neutron OS is a toy operating system. Maybe it will be useful, but probably it will just stand as an example. I'm writing it with a 64-bit architecture, so run it on a 64-bit system (or a 64-bit emulator).

## Key Points
Strikethrough indicates it has been completed. Currently my main concern is with the terminal-based interface.

* ~Bootable long mode system~
* ~Terminal output~
* Keyboard input
* ~Mouse input~
* Terminal-based graphical interface
* Interrupts and proper event handling
* BIOS interrupts
* Memory filesystem and disk buffering
* Userspace, functional 'applications'

## Releases and Building
Live debug releases are available in the ```build/``` directory. Working versions are released whenever I've made a significant move forward: see the [releases page](https://github.com/JkyProgrammer/Neutron-OS/releases).

If you want to build for yourself, use the Makefile.
* ```make all``` will simply build the 'kernel-x86_64.bin' file from the source.
* ```make clean``` clean the build folder.
* ```make run``` will build the 'kernel-x86_64.bin' file and then package it up into an 'iso' file. It will also then launch Neutron using a qemu emulator (with serial output directed to the ```log/serial/``` directory).
* ```make debug``` is just make run, except the running options are set up to connect the OS to gdb for debugging.
* ```make cleanserial``` (unsurprisingly) cleans the logs from the serial log directory).


## Contributions
Based partly on its predecessor OtterOS, which was contributed to by **DaBatchMan**.
Copyright **JkyProgrammer** 2018-2019
