#ifndef __FILE_IO__H
#define __FILE_IO__H

#include "std_lib.h"
#include "filesystem.h"
#include "string.h"

void write(struct file_metadata *metadata, enum fs_retcode *return_code);
void read(struct file_metadata *metadata, enum fs_retcode *return_code);

#endif