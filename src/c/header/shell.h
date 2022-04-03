#include "kernel.h"

#ifndef _SHELL_H_
#define _SHELL_H_

#define MAX_LEN 25

void shell();
void command(char *input_buf, byte *current_dir);
void argSplitter(char *input_buf, char *command, char* arg1, char *arg2);

void commandParser(char *command, char *argument);

void cd(byte *parentIndex, char *dir);

void ls(byte parentIndex);

void cat(byte parentIndex, char *path);

void cp(byte parentIndex, char *resourcePath, char *destinationPath);

void printCWD(char *path_str, byte current_dir);

void error_code(int error_code);

void mkdir(byte parentIndex, struct file_metadata *fileInfo);

void mv(byte parentIdx, char *source, char *target);
byte readPath(char *path_str, byte current_dir, struct node_filesystem node_fs_buffer, struct sector_filesystem secotr_fs_buffer);

#endif