// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char buf[128];
    clearScreen();
    makeInterrupt21();
    printString("Halo dunia!\r\n");
    readString(buf);
    printString(buf);

    while (true);
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
        interrupt(0x10, 0x0E00 + c,0,0,0);
        i++;
        c = string[i];
    }
}

void readString(char *string) {
    int i = 0;
    char c = string[i];
    while (c != '\0') {
        interrupt(0x10, 0x0E00 + c,0,0,0);
        i++;
        c = string[i];
    }
}

void clearScreen() {
}