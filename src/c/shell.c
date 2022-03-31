#include "header/shell.h"

void printCWD(char* path_str, char* current_dir) {
    
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