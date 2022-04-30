#ifndef __STRING__H
#define __STRING__H

#include "std_type.h"
#include "textio.h"

unsigned int strlen(char *string);
// Mengembalikan panjang suatu null terminated string

bool strcmp(char *s1, char *s2);
// Mengembalikan true jika string sama

void strcpy(char *dst, char *src);
// Melakukan penyalinan null terminated string

int strparsing(char *input, char output[8][256]);

#endif