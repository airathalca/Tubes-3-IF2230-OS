// #include "std_lib.c"
#include "header/filesystem.h"
#include <stdio.h>
#include <string.h>

int main(){
  char a[2] = {'a', 'b'};
  char b[2] = {'b', 'a'};
  printf("%d", strcmp(a, b));
  
  printf("%d", strcmp(a, b));
}