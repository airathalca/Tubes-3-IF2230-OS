#ifndef __FILE_IO__H
#define __FILE_IO__H

#include "std_lib.h"
#include "filesystem.h"
#include "string.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);
void write(struct file_metadata *metadata, enum fs_retcode *return_code);
void read(struct file_metadata *metadata, enum fs_retcode *return_code);
void readSector(byte *buffer, int sector_number);
void writeSector(byte *buffer, int sector_number);

#endif