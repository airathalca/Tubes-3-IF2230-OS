# uSUSbuntu OS

This project is made to fulfill the 1st, 2nd, and 3rd milestone for the Operating System. It is a simple os that can do simple command like ls, mkdir, cp, cd, cat, and mv. It also implemented by using multiprogramming and message passing

## Made by

| Name                           |   Nim    |
| ------------------------------ | :------: |
| Muhammad Garebaldhie ER Rahman | 13520029 |
| Rayhan Kinan Muhannad          | 13520065 |
| Aira Thalca Avila Putra        | 13520101 |

## Screenshots

![image](https://user-images.githubusercontent.com/73151449/166109915-f7bd74c9-4722-4497-b010-348d8d9a8cf7.png)
![image](https://user-images.githubusercontent.com/73151449/166109954-84f7dcaa-4d5b-4a8e-b2e5-7cc9be9da866.png)


## Technologies Used

1. [VirtualBox](https://www.virtualbox.org/)
2. [Window Subsytem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install)
3. [Ubuntu 20.04 LTS](https://releases.ubuntu.com/20.04/)
4. [Nasm](https://www.nasm.us/)
5. [bcc](https://bochs.sourceforge.io/)
6. bochs
7. c

## Features

This OS was in implementation from the boilerplate given by sister19. In this OS you can use simple command like `cat` to read file, `ls` to list file and many more. To see more command check the command section. This OS is made using bcc, bochs, c, and asm. 

## Program Features
1. Kernel
2. Shell
4. cd
5. cp
6. cat
7. ls
8. mv
9. mkdir

## Setup

1. Install all the requirements in the technologies section

```
sudo apt update
sudo apt install nasm bcc bin86 bochs bochs-x make
```

2. Run the virtual machine using VirtualBox (Another option by using [WSL2](https://github.com/Sister19/WSL-Troubleshoot))
3. clone the github repository by using `git clone https://github.com/Sister19/tugas-besar-os-ususbuntu`
4. run `make all run` and viola! the bochs emulator should pop up and the os are ready to be used

## Command
1. `clear` - to clear the screen
2. `cd <target_dir>` - move from current directory to target dir
3. `ls [folder]` - list all the files or folder in folder name
4. `mv <file or folder> <target_dir>/[new_name]` - move file or folder in current dir to the destination and be able to rename the file if the argument is specified
5. `mkdir <folder_name>` - to create new folder in current dir
6. `cat <file_name>` - to read and display file
7. `cp <file or folder> <target>` - copy file to target dir if no folder exists will rename to target

8. `[OPTIONAL]` - if not specified refer to cwd 
9. `<MANDATORY>` - this argument is must in order to run

## Usage

1. After you run the OS the clearFunction will run by default and it will clear the screen leaving the shell with the location of curdir
2. You can type any character, or command in that will go to the buffer with 128 byte size.
3. If you pressed enter, the text you typed before will be process as command.

## References

1. [Interrupt](http://www.oldlinux.org/Linux.old/docs/interrupts/int-html/int.htm)
2. [Bios and Dos](https://jbwyatt.com/253/emu/8086_bios_and_dos_interrupts.html)
3. [Milestone1 Doc](https://docs.google.com/document/d/1x9g3kspefka_vBl8JseBROv8f7cQdfEq-59W0jOqmo4/edit#)
4. [Milestone2 Doc](https://docs.google.com/document/d/12b8gwm5RZyFfdXUKDzauFuJMtFQ2QCcKEqiJBa7sORU/edit#)
5. [Milestone3 Doc](https://docs.google.com/document/d/1-ofSIjvQA089TuUklTesTLeBeCW_4HJiJOdDCAicop8/edit?pli=1#)
