#ifndef __TEXT_IO__H
#define __TEXT_IO__H

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void puts(char *string);
void gets(char *string);

#endif