# uSUSbuntu OS

This project is made to fulfill the 1st, 2nd, and 3rd milestone for the Operating System. It is a simple os that can do simple command like ls, mkdir, cp, cd, cat, and mv. It also implemented by using multiprogramming and message passing

## Made by

| Name                           |   Nim    |
| ------------------------------ | :------: |
| Muhammad Garebaldhie ER Rahman | 13520029 |
| Rayhan Kinan Muhannad          | 13520065 |
| Aira Thalca Avila Putra        | 13520101 |

## Screenshots

![image](https://user-images.githubusercontent.com/63847012/161774200-b41ea274-861f-426f-afa0-11670bc6ed01.png)
![image](https://user-images.githubusercontent.com/63847012/161774340-fcd429be-9e05-4b83-a82a-47ed319cd181.png)

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
1. kernel
2. shell
4. cd
5. cp
6. cat
7. ls
8. mv
9. mkdir
10. execute program locally & globally
11. multiple program with and without message passing

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
8. `./<file_name>` - to execute file in current dir

9. `[OPTIONAL]` - if not specified refer to cwd 
10. `<MANDATORY>` - this argument is must in order to run

## Usage

1. Run the kernel program and a welcome page will be displayed. Press any key to clear the screen and use the program.
2. After you run the OS the clearFunction will run by default and it will clear the screen leaving the shell with the location of curdir
3. You can type any character, or command in that will go to the buffer with 128 byte size.
4. If you pressed enter, the text you typed before will be process as command.

## References

1. [Interrupt](http://www.oldlinux.org/Linux.old/docs/interrupts/int-html/int.htm)
2. [Bios and Dos](https://jbwyatt.com/253/emu/8086_bios_and_dos_interrupts.html)
3. [Milestone1 Doc](https://docs.google.com/document/d/1x9g3kspefka_vBl8JseBROv8f7cQdfEq-59W0jOqmo4/edit#)
4. [Milestone2 Doc](https://docs.google.com/document/d/12b8gwm5RZyFfdXUKDzauFuJMtFQ2QCcKEqiJBa7sORU/edit#)
5. [Milestone3 Doc](https://docs.google.com/document/d/1-ofSIjvQA089TuUklTesTLeBeCW_4HJiJOdDCAicop8/edit?pli=1#)
