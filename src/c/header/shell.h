#include "kernel.h"

#ifndef _SHELL_H_
#define _SHELL_H_

#define MAX_LEN 25

void shell();
void command_type(char *command, byte *current_dir, char*arg1, char*arg2, int *ret_code);
void argSplitter(char *input_buf, char *command, char* arg1, char *arg2);
void cd(byte *parentIndex, char *dir, int *ret_code);
void ls(byte parentIndex, char *arg1, int *ret_code);
void cat(byte parentIndex, char *arg1, int *ret_code);
void cp(byte parentIndex, char *resourcePath, char *destinationPath, int *ret_code);
void printCWD(char *path_str, byte current_dir);
void error_code(int error_code);
void mkdir(byte parentIndex, char *arg1, int *ret_code);
void mv(byte parentIdx, char *source, char *target, int *ret_code);

byte read_absolute_path(char *path_str, enum fs_retcode *ret_code);

#endif