#include "kernel.h"

#ifndef _SHELL_H_
#define _SHELL_H_

#define MAX_LEN 25

void shell();

void commandParser(char *command, char *argument);

void cd(char *parentIndex, char *dir, char *newCWD);

void ls(char parentIndex);

void cat(char parentIndex, char *path);

void cp(char parentIndex, char *resourcePath, char *destinationPath);

void printCWD(char *path_str, char *current_dir);

void error_code(int error_code);

void mkdir(byte parentIndex, struct file_metadata *fileInfo);

#endif