#include "header/shell.h"

void printCWD(char* path_str, char* current_dir) {
  struct node_filesystem node_fs_buffer;
  struct sector_filesystem sector_fs_buffer;
  int i = 0;
  int idx = 0;
  byte parent = 0;
  byte nodeIndex[64];
  int curLen = 0;
  int j = 0;
  char *iteratorPath;
  int sizeNow;
  readSector(&node_fs_buffer, FS_NODE_SECTOR_NUMBER);
  readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 1);
  readSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER);
  //1. mane cari yang current_dir nya sama
  while(i < 64) {
    if(strcmp(current_dir, node_fs_buffer.nodes[i].name)) {
      parent = node_fs_buffer.nodes[i].parent_node_index;
      nodeIndex[idx++] = i;
      break;
    }
  }
  
  //simpen semuanya di nodeIndex
  while(parent != FS_NODE_P_IDX_ROOT){
    if (parent == (j % 64)) {
      nodeIndex[idx++] = j;
      parent = node_fs_buffer.nodes[j].parent_node_index;
    }
    j++;
  }
  
  //copy semuanya ke path_str
  while(idx > 0) {
    iteratorPath = node_fs_buffer.nodes[nodeIndex[idx]].name;
    sizeNow = strlen(iteratorPath);
    //sizenow, sizenow + 1, sizenow + 2
    iteratorPath[sizeNow + 1] = "/";
    //tambahin nol biar nandain null byte
    iteratorPath[sizeNow + 2] = '\0';
    strcpy(path_str + curLen, iteratorPath);
    curLen += (sizeNow + 1);
    idx--;
  }
  strcpy(path_str + curLen, current_dir);
  printString(path_str);
}

void shell() {
    char input_buf[64];
    char path_str[128];
    byte current_dir = FS_NODE_P_IDX_ROOT;

    while (true) {
        printString("OSIF2230:");
        printCWD(path_str, current_dir);
        printString("$ ");
        readString(input_buf);
    }

    if (strcmp(input_buf, "cd")) {
      
    } else if (strcmp(input_buf, "ls")) {
        
    } else if (strcmp(input_buf, "mv")) {

    } else if (strcmp(input_buf, "mkdir")) {

    } else if (strcmp(input_buf, "cat")) {

    } else if (strcmp(input_buf, "cp")) {

    } else {
        printString("Unknown command\r\n");
    }
}

void cd(char *parentIndex, char *dir, char *newCWD) {
    
}

void ls(char parentIdx) {
    struct node_filesystem node_fs_buffer;
    int i = 0;

    readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    
    while (i < 64) {
        if (node_fs_buffer.nodes[i].parent_node_index == parentIdx && 
        node_fs_buffer.nodes[i].name[0] != '\0') {
            readString(node_fs_buffer.nodes[i].name);
            readString("\n");
        }
        i++;
    }
}

void cat(char parentIndex, char *path) {
}