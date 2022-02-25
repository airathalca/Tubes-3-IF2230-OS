// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char buf[128];
    makeInterrupt21();
    interrupt(0x16, 0, 0, 0, 0);
    printString("Welcome to uSUSbuntu OS\r\n");
    while(true){
      handleInterrupt21(1, buf);
      handleInterrupt21(0, buf);
      // clearScreen();
    }
    clearScreen();
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    switch (AX) {
        case 0x0:
            printString(BX);
            break;
        case 0x1:
            readString(BX);
            break;
        default:
            printString("Invalid Interrupt");
    }
}

void printString(char *string) {
    int i = 0;
    char c = string[i];
    
    while (c != '\0') {
        interrupt(0x10, 0x0E00 + c, 0, 0, 0);
        i++;
        c = string[i];
    }
}

void readString(char *string) {
    bool check = true;
    char c = '\0';
    int i = 0;
    while (check) {
        c = interrupt(0x16, 0x0, 0, 0, 0);

        if (c == '\r' || c == '\n') {
            string[i] = '\0';
            check = false;
            printString("\r\n");
            // printString("\x13\x);

        } else if (c == '\b') {
            interrupt(0x10, 0x0E00 + '\b', 0, 0, 0);
            interrupt(0x10, 0x0E00 + ' ', 0, 0, 0);
            interrupt(0x10, 0x0E00 + '\b', 0, 0, 0);
            string[i] = '\0';
            if(i > 0){
              i--;
            }
        } else {
            string[i] = c;
            interrupt(0x10, 0x0E00 + c, 0, 0, 0);
            i++;
        }
    }
    // interrupt(0x10, 0x2000, 0, 0, ((interrupt(0x10, 0x3000, 0) << 2) + 0x1) >> 2);
}

//getPosition
// interrupt(0x10, 0x2000, 0, 0, ((interrupt(0x10, 0x3000, 0) << 2) + 0x1) >> 2);

void clearScreen() {
  interrupt(0x10, 0x0003);
}