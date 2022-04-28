#include "header/textio.h"

void puts(char *string){
  interrupt(0x21, 0, string, 0, 0);
}
void gets(char *string){
  interrupt(0x21, 0x1, string, 0, 0);
}

void putsHexa(int c){
  interrupt(0x21, 0x9, c, 0, 0);
}