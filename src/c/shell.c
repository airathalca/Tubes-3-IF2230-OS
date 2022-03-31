#include "header/kernel.h"

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