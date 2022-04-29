#ifndef __PROGRAM__H
#define __PROGRAM__H

#include "message.h"
#include "filesystem.h"
#include "utils.h"
#include "textio.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void exec(struct message *m, int segment);
void exit();
void error_code(int err_code, char*command, char*arg1, char*arg2);

bool checkArgs(char *filename, int *ret_code);

#endif