#ifndef __SHELL__H
#define __SHELL__H

#include "textio.h"
#include "string.h"
#include "stdlib.h"
#include "filesystem.h"
#include "fileio.h"
#include "message.h"
#include "utils.h"
#include "program.h"

#define MAX_LEN 25

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);
void printCWD(char *path_str, byte current_dir);
bool checkArgs(char *filename, int *ret_code);
// byte read_relative_path(byte parentIdx, char *path_str, enum fs_retcode *ret_code);

// void command_type(char *command, byte *current_dir, char*arg1, char*arg2, enum fs_retcode *ret_code);
// void argSplitter(char *input_buf, char *command, char* arg1, char *arg2);
// void cd(byte *parentIndex, char *dir, enum fs_retcode *ret_code);
// void cp(byte parentIndex, char *resourcePath, char *destinationPath, enum fs_retcode *ret_code);
// void error_code(int error_code, char *command, char* arg1, char*arg2);
// void mkdir(byte parentIndex, char *arg1, enum fs_retcode *ret_code);
// void mv(byte parentIdx, char *source, char *target, enum fs_retcode *ret_code);


#endif