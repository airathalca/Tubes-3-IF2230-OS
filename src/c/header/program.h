#ifndef __PROGRAM__H
#define __PROGRAM__H

#include "message.h"
#include "filesystem.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void exec(struct message *m);
void exit(struct message *m);

#endif