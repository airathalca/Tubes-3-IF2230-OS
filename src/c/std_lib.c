#include "header/std_lib.h"

// Opsional : Tambahkan implementasi kode C
int div (int a, int b) {
  if (a == b) {
    return 1;
  }
  int res = 0;
  while (a >= b) {
    a -= b;
    res++;
  }
  return res;
}

int mod (int a, int n) {
  while (a >= n) {
    a -= n;
  }
  return a;
}

unsigned int strlen(char *string) {
  int res = 0;
  while (string[res] != 0x0) {
    res++;
  }
  return res;
}

bool strcmp(char *s1, char *s2) {
  if (strlen(s1) != strlen(s2)) {
    return false;
  }
  int i;
  for (i = 0;i < strlen(s1);i++) {
    if (s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}