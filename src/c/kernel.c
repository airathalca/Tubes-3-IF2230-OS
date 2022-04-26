// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
// ek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    fillKernelMap();
    clearScreen();
    makeInterrupt21();
    welcome();
    shell();
}

void welcome(){
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    switch (AX) {
        case 0x0:
            printString(BX);
            break;
        case 0x1:
            readString(BX);
            break;
        case 0x2:
            readSector(BX, CX);
            break;
        case 0x3:
            writeSector(BX, CX);
            break;
        case 0x4:
            read(BX, CX);
            break;
        case 0x5:
            write(BX, CX);
            break;
        case 0x6:
            executeProgram(BX, CX);
        default:
            printString("Invalid Interrupt");
    }
}

void printString(char *string) {
    int i = 0;
    char c = string[i];
    byte color = 0xD;
    while (c != '\0') {
        if(c == '\n')interrupt(0x10, 0x0E00 + '\r', 0, 0, 0);
        interrupt(0x10, 0x0E00 + c, 0, 0, 0);
        i++;
        c = string[i];
    }
}

// void printHexa(char n)
// {
// 	char number[3];
// 	number[2] = 0;
// 	number[0] = div(n, 16);
// 	number[0] = number[0] >= 10 ? (number[0]-10) + 'A' : number[0] + '0';
// 	number[1] = mod(n, 16);
// 	number[1] = number[1] >= 10 ? (number[1]-10) + 'A' : number[1] + '0';
// 	printString(number);
// }

void readString(char *string) {
    bool check = true;
    char c = '\0';
    int i = 0;
    int j = 0;
    while (check) {
        //read character <- user input
        c = interrupt(0x16, 0x0, 0, 0, 0);
        if (c == '\r' || c == '\n') {
            string[i++] = '\0';
            check = false;
            printString("\r\n");
        } else if (c == '\b') {
            if (i > 0){
              interrupt(0x10, 0x0E00 + '\b', 0, 0, 0);
              interrupt(0x10, 0x0E00 + ' ', 0, 0, 0);
              interrupt(0x10, 0x0E00 + '\b', 0, 0, 0);
              string[i] = '\0';
              i--;
            }

        } else if (c == '\t') {
          for(j = 0; j < 4; j++){
            string[i++] = ' ';
            interrupt(0x10, 0x0E00 + ' ', 0, 0, 0);
          }
        } 
        else {
            string[i] = c;
            interrupt(0x10, 0x0E00 + c, 0, 0, 0);

            if(i < 127){
              i++;
            }
        }
    }
}

void clearScreen() {
  interrupt(0x10, 0x0200, 0, 0, 0);
  interrupt(0x10, 0x0600, 0x0f00, 0, 0x184f);
}

void readSector(byte *buffer, int sector_number) {
    interrupt(0x13, 0x0201,buffer,div(sector_number, 36) << 8 | mod(sector_number, 18) + 1,mod(div(sector_number, 18), 2) << 8);
}

void writeSector(byte *buffer, int sector_number) { 
    interrupt(0x13, 0x0301, buffer, div(sector_number, 36) << 8 | mod(sector_number, 18) + 1, mod(div(sector_number, 18), 2) << 8);
}


void fillKernelMap(){
  struct map_filesystem map_fs_buffer;
  int i;
  // Load filesystem map
  readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
  for(i = 0; i < 512; i++) {
    if(i < 16 || i > 256){
      map_fs_buffer.is_filled[i] = true;
    }
  }
  writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER); 
}


// EXECUTE PROGRAM bikin beledak karena kernel sizenya kurang
void executeProgram(struct file_metadata *metadata, int segment) {
  enum fs_retcode fs_ret;
  byte buf[8192];
  int i = 0;

  metadata->buffer = buf;
  read(metadata, &fs_ret);
  if (fs_ret == FS_SUCCESS) {
    for (i = 0; i < 8192; i++) {
      if (i < metadata->filesize) {
        putInMemory(segment, i, metadata->buffer[i]);
      } 
      else putInMemory(segment, i, 0x00);
    }
    launchProgram(segment);
  }else {
    printString("exec: file not found\r\n");
  } 
}