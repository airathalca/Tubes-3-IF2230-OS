// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
// ek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    char buf[128];
    //call the bootloader
    fillKernelMap();
    clearScreen();
    makeInterrupt21();
    welcome();

    shell();
}

void welcome(){
    interrupt(0x10, 0x0200, 0, 0, 0x0307);
    printString("       ____  _   _ ____  _                 _            ___  ____\r\n");
    interrupt(0x10, 0x0200, 0, 0, 0x0407);
    printString(" _   _/ ___|| | | / ___|| |__  _   _ _ __ | |_ _   _   / _ \\/ ___|\r\n");
    interrupt(0x10, 0x0200, 0, 0, 0x0507);
    printString("| | | \\___ \\| | | \\___ \\| '_ \\| | | | '_ \\| __| | | | | | | \\___ \\\r\n");
    interrupt(0x10, 0x0200, 0, 0, 0x0607);
    printString("| |_| |___) | |_| |___) | |_) | |_| | | | | |_| |_| | | |_| |___) |\r\n");
    interrupt(0x10, 0x0200, 0, 0, 0x0707);
    printString(" \\__,_|____/ \\___/|____/|_.__/ \\__,_|_| |_|\\__|\\__,_|  \\___/|____/\r");
    interrupt(0x10, 0x0200, 0, 0, 0x0928 - 0x0C);
    printString("Welcome to uSUSbuntu OS\r\n");
    interrupt(0x10, 0x0200, 0, 0, 0x0A28 - 0x12);
    printString("Press enter any key to get started\r\n\n");
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
        default:
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
    int j = 0;
    while (check) {
        //read character <- user input
        c = interrupt(0x16, 0x0, 0, 0, 0);
        if (c == '\r' || c == '\n') { //carriage and newline
            string[i++] = '\0';
            check = false;
            printString("\r\n");

        } else if (c == '\b') { //backspace

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
        else { //insert to buffer but don't exceed the boundary
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
  int sector_read_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector   = mod(sector_number, 18) + 1;  // CL

    head  = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                                // DL

    interrupt(
        0x13,                       // Interrupt number
        0x0200 | sector_read_count, // AX
        buffer,                     // BX
        cylinder | sector,          // CX
        head | drive                // DX
    );
}

void writeSector(byte *buffer, int sector_number) {
  int sector_read_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector   = mod(sector_number, 18) + 1;  // CL

    head  = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                                // DL

    interrupt(
        0x13,                       // Interrupt number
        0x0300 | sector_read_count, // AX
        buffer,                     // BX
        cylinder | sector,          // CX
        head | drive                // DX
    );

}


void fillKernelMap(){
  struct map_filesystem map_fs_buffer;
  int i;
  // Load filesystem map
  readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
  for(i = 0; i < 512; i++) {
    if(i < 16 || i > 256){
      map_fs_buffer.is_filled[i] = true;
    } else {
      map_fs_buffer.is_filled[i] = false;
    }
  }
  writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER); 
}

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
  // Tambahkan tipe data yang dibutuhkan
  struct node_filesystem   node_fs_buffer;
  struct node_entry        node_buffer;
  struct sector_filesystem sector_fs_buffer;
  struct sector_entry      sector_entry_buffer;
  int i = 0;
  int j = 0;
  bool found = false;

  // Masukkan filesystem dari storage ke memori buffer
  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  readSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER);

  // 1. Cari node dengan nama dan lokasi yang sama pada filesystem.
  //    Jika ditemukan node yang cocok, lanjutkan ke langkah ke-2.
  //    Jika tidak ditemukan kecocokan, tuliskan retcode FS_R_NODE_NOT_FOUND
  //    dan keluar. 

  while (i < 64 && !found) {
    if (strcmp(node_fs_buffer.nodes[i].name, metadata->node_name) && metadata->parent_index == node_fs_buffer.nodes[i].parent_node_index){
      found = true;
    } else {
      i++;
    }
  }

  if (!found){
    *return_code = FS_R_NODE_NOT_FOUND;
    return;
  }
  
  // 2. Cek tipe node yang ditemukan
  //    Jika tipe node adalah file, lakukan proses pembacaan.
  //    Jika tipe node adalah folder, tuliskan retcode FS_R_TYPE_IS_FOLDER
  //    dan keluar.

  if (node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
    *return_code = FS_R_TYPE_IS_FOLDER;
    return;
  }

  // Pembacaan
  // 1. memcpy() entry sector sesuai dengan byte S
  memcpy(&sector_entry_buffer, &sector_fs_buffer.sector_list[node_fs_buffer.nodes[i].sector_entry_index], 16);

  // 2. Lakukan iterasi proses berikut, i = 0..15
  // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
  // 4. Jika byte bernilai 0, selesaikan iterasi
  // 5. Jika byte valid, lakukan readSector() 
  //    dan masukkan kedalam buffer yang disediakan pada metadata
  // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
  printString(&sector_entry_buffer);
  clear(metadata->buffer, metadata->filesize);

  for(j = 0; j < 16; j++){
    if(sector_entry_buffer.sector_numbers[j]){
      readSector(metadata->buffer[j * 512], sector_entry_buffer.sector_numbers[j]);
    } else {
      break;
    }
  }

  // 7. Tulis retcode FS_SUCCESS dan ganti filesize 
  // pada akhir proses pembacaan.

  metadata->filesize = j * 512;
  *return_code = FS_SUCCESS;
}

void write(struct file_metadata *metadata, enum fs_retcode *return_code) {
  struct node_filesystem   node_fs_buffer;
  struct sector_filesystem sector_fs_buffer;
  struct map_filesystem    map_fs_buffer;
  int node_idx = 0;
  int emptyNode = 0;
  int emptySector = 0;
  int i = 0;
  int j = 0;
  int k = 0;
  int l;
  struct sector_entry buffer;
  bool found = false;
  byte parent;
  unsigned int sizenow;

  readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
  readSector(&node_fs_buffer.nodes, FS_NODE_SECTOR_NUMBER);
  readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 1);
  readSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER);

  printString("1");
  // 1. Cari node dengan nama dan lokasi parent yang sama pada node.
  //    Jika tidak ditemukan kecocokan, lakukan proses ke-2.
  //    Jika ditemukan node yang cocok, tuliskan retcode 
  //    FS_W_FILE_ALREADY_EXIST dan keluar. 
  while (i < 64 && !found) {
    if (strcmp(node_fs_buffer.nodes[i].name, metadata->node_name) && 
    metadata->parent_index == node_fs_buffer.nodes[i].parent_node_index){
      found = true;
    }
    i++;
  }
  // printString(node_fs_buffer.nodes[i].name);
  printString("\r\n");
  if(strcmp(node_fs_buffer.nodes[i].name, metadata->node_name)){
    printString("AAAAAA\r\n");
  }
  printString(&metadata->node_name);
  printString("\r\n");
  if (found) {
    *return_code = FS_W_FILE_ALREADY_EXIST;
    return;
  }

  if (strlen(metadata->node_name) > 13) {
    *return_code = FS_W_NOT_ENOUGH_STORAGE;
    return;
  }

  printString("2");
  // 2. Cari entri kosong pada filesystem node dan simpan indeks.
  //    Jika ada entry kosong, simpan indeks untuk penulisan.
  //    Jika tidak ada entry kosong, tuliskan FS_W_MAXIMUM_NODE_ENTRY
  //    dan keluar.

  for (j = 0; j < 64; j++) {
    if (node_fs_buffer.nodes[j].name[0] == '\0') {
      emptyNode = j;
      break;
    }
  }

  if (j == 64) {
    *return_code = FS_W_MAXIMUM_NODE_ENTRY;
    return;
  }

  // 3. Cek dan pastikan entry node pada indeks P adalah folder.
  //    Jika pada indeks tersebut adalah file atau entri kosong,
  //    Tuliskan retcode FS_W_INVALID_FOLDER dan keluar.
  printString("TEST");
  if (metadata->parent_index != FS_NODE_P_IDX_ROOT && node_fs_buffer.nodes[metadata->parent_index].sector_entry_index != FS_NODE_S_IDX_FOLDER) {
    *return_code = FS_W_INVALID_FOLDER;
    return;
  }

  // 4. Dengan informasi metadata filesize, hitung sektor-sektor 
  //    yang masih kosong pada filesystem map. Setiap byte map mewakili 
  //    satu sektor sehingga setiap byte mewakili 512 bytes pada storage.
  //    Jika empty space tidak memenuhi, tuliskan retcode
  //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
  //    Jika ukuran filesize melebihi 8192 bytes, tuliskan retcode
  //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
  //    Jika tersedia empty space, lanjutkan langkah ke-5.

  printString(&metadata->filesize);

  if (metadata->filesize > 8192) {
    *return_code = FS_W_NOT_ENOUGH_STORAGE;
    return;
  }

  sizenow = metadata->filesize;

  for (i = 0; i < 512; i++) {
    if (!map_fs_buffer.is_filled[i]) {
      sizenow -= 512;
    }
  }

  if (sizenow > 0) {
    *return_code = FS_W_NOT_ENOUGH_STORAGE;
    return;
  }

  printString("3");

  // 5. Cek pada filesystem sector apakah terdapat entry yang masih kosong.
  //    Jika ada entry kosong dan akan menulis file, simpan indeks untuk 
  //    penulisan.
  //    Jika tidak ada entry kosong dan akan menulis file, tuliskan
  //    FS_W_MAXIMUM_SECTOR_ENTRY dan keluar.
  //    Selain kondisi diatas, lanjutkan ke proses penulisan.

  if(metadata->filesize > 0){
    for (i = 0; i < 32; i++) {
      if (sector_fs_buffer.sector_list[i].sector_numbers[0] == '\0') {
        emptySector = i;
        break;
      }
    }

    if (i == 32) {
      *return_code = FS_W_MAXIMUM_SECTOR_ENTRY;
      return;
    }
  }


  // Penulisan
  // 1. Tuliskan metadata nama dan byte P ke node pada memori buffer
  // 2. Jika menulis folder, tuliskan byte S dengan nilai 
  //    FS_NODE_S_IDX_FOLDER dan lompat ke langkah ke-8
  // 3. Jika menulis file, tuliskan juga byte S sesuai indeks sector
  // 4. Persiapkan variabel j = 0 untuk iterator entry sector yang kosong
  // 5. Persiapkan variabel buffer untuk entry sector kosong
  // 6. Lakukan iterasi berikut dengan kondisi perulangan (penulisan belum selesai && i = 0..255)
  //    1. Cek apakah map[i] telah terisi atau tidak
  //    2. Jika terisi, lanjutkan ke iterasi selanjutnya / continue
  //    3. Tandai map[i] terisi
  //    4. Ubah byte ke-j buffer entri sector dengan i
  //    5. Tambah nilai j dengan 1
  //    6. Lakukan writeSector() dengan file pointer buffer pada metadata 
  //       dan sektor tujuan i
  //    7. Jika ukuran file yang telah tertulis lebih besar atau sama dengan
  //       filesize pada metadata, penulisan selesai
  
  // 7. Lakukan update dengan memcpy() buffer entri sector dengan 
  //    buffer filesystem sector
  // 8. Lakukan penulisan seluruh filesystem (map, node, sector) ke storage
  //    menggunakan writeSector() pada sektor yang sesuai
  // 9. Kembalikan retcode FS_SUCCESS


  printString("4");

  strcpy(node_fs_buffer.nodes[emptyNode].name, metadata->buffer);
  node_fs_buffer.nodes[emptyNode].parent_node_index = metadata->parent_index;

  if (metadata->filesize == 0) {
    node_fs_buffer.nodes[emptyNode].sector_entry_index = FS_NODE_S_IDX_FOLDER;
  } 

  else {
    //i: sector (i), k: iterator entry kosong (j), l : iterator biasa
    node_fs_buffer.nodes[emptyNode].sector_entry_index = emptySector;
    for (l = 0; l < 256; l++) {

      if (map_fs_buffer.is_filled[l]) {
        continue;
      }

      map_fs_buffer.is_filled[l] = true;
      buffer.sector_numbers[k] = l;
      writeSector(metadata->buffer + k * 512, l);
      k++;
      if (metadata->filesize <= 512 * k) {
        break;
      }
    }

    memcpy(&sector_fs_buffer.sector_list[i], &buffer, k);
  }

  printString("5");

  writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
  writeSector(&node_fs_buffer, FS_NODE_SECTOR_NUMBER);
  writeSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 1);
  writeSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER);
  printString("6");
}
