#include "header/std_lib.h"

// Opsional : Tambahkan implementasi kode C
int div (int a, int b) {
  int res = 0;
  while (a>=b) {
    a -= b;
    res++;
  }
  return res;
}

int mod (int a, int n) {
  return a - div(a,n) * n;
}

void memcpy(byte *dest, byte *src, unsigned int n) {
  clear(dest, strlen(dest));
  while (n--) {
    *dest++ = *src++;
  }
}

void clear(byte *ptr, unsigned int n) {
  while (n > 0) {
    *ptr++ = 0;
    n--;
  }
}