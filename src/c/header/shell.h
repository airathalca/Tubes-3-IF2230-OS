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

#endif