// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char buf[128];
    //call the bootloader
    makeInterrupt21();
    printString("Welcome to uSUSbuntu OS\r\n");
    printString("Press enter any key to get started\r\n");
    //wait for user input
    while(true){
      interrupt(0x21, 1, buf, 0, 0);
      interrupt(0x21, 0, buf, 0, 0);
    }
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
            clearScreen();
            printString("Invalid Interrupt");
    }
}

//0xB8000 <- charater in top left (x = 0, y = 0)
//0xB000 <- start segment
//0x8000 + (80*y + x) * 2 <- y
//karna ukurannya 80x25 berarti setiap huruf pindah 80

void printString(char *string) {
    int i = 0;
    char c = string[i];
    byte color = 0xD;
    //obvious way
    while (c != '\0') {
        interrupt(0x10, 0x0E00 + c, 0, 0, 0);
        i++;
        c = string[i];
    }

    //put memory way dont work :( we must know the position of current curosor and then + 1
//     while(c != '\0'){
//       putInMemory(0xB000, 0x8000 + 2*i, c);
//       putInMemory(0xB000, 0x8001 + 2*i, color);
//       i++;
//     }
// }
}

void readString(char *string) {
    bool check = true;
    char c = '\0';
    int i = 0;
    while (check) {
        //read character <- user input
        c = interrupt(0x16, 0x0, 0, 0, 0);
        if (c == '\r' || c == '\n') { //carriage and newline
            string[i++] = '\r';
            string[i++] = '\n';
            string[i++] = '\0';
            check = false;
            printString("\r\n");
        } else if (c == '\b') { //backspace
            interrupt(0x10, 0x0E00 + '\b', 0, 0, 0);
            interrupt(0x10, 0x0E00 + ' ', 0, 0, 0);
            interrupt(0x10, 0x0E00 + '\b', 0, 0, 0);
            string[i] = '\0';
            if(i > 0){
              i--;
            }
        } else { //insert to buffer but don't exceed the boundary
            string[i] = c;
            interrupt(0x10, 0x0E00 + c, 0, 0, 0);
            if(i <= 126){
              i++;
            }
        }
    }
}

void clearScreen() {
  interrupt(0x10, 0x0003);
}