#include "header/string.h"

unsigned int strlen(char *string) {
  int res = 0;
  while (string[res] != 0x0) {
    res++;
  }
  return res;
}

bool strcmp(char *s1, char *s2) {
  int i = 0;
  
  if (strlen(s1) != strlen(s2)) {
    return false;
  }
  for (i = 0; i < strlen(s1); i++) {
    if (s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}

void strcpy(char *dst, char *src) {
  clear(dst, strlen(dst));
  while ((*dst++ = *src++) != '\0');
}