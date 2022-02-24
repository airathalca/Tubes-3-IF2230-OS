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