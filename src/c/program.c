#include "header/program.h"

//exec tuh intinya ngejalanin program di segment tertentu
void exec(struct message *m, int segment){
  struct file_metadata file;
  file.node_name = m->arg1;
  file.parent_index = 0;
  interrupt(0x21, 0x6, &file, segment, 0);
}

void exit(){
  struct message now;
  struct message next;

  //ambil message abis itu set next dir 
  getMessage(&now, getCurrentSegment());
  getMessage(&next, now.next_program_segment);
  next.current_directory = now.current_directory;
  sendMessage(&next, now.next_program_segment);
  
  // puts("MENUJU: ");
  // putsHexa(next.next_program_segment);
  // puts("\r\n");
  // puts(next.arg1);
  // puts("\r\n");
  exec(&next, now.next_program_segment);
};

bool checkArgs(char *filename, int *ret_code) {
  if(filename[0] == '\0'){
      *ret_code = FS_R_NODE_NOT_FOUND;
      return false;
    }
  if(strlen(filename) > 13){
    *ret_code = FS_W_NOT_ENOUGH_STORAGE;
    return false;
  }
  return true;
}

void error_code(int err_code, char*command, char*arg1, char*arg2){
  int arg1_len = 0;
  int arg2_len = 0;
  if(err_code != 0){
    puts(command);
    puts(": ");
  }
  arg1_len = strlen(arg1);
  arg2_len = strlen(arg2);
  switch (err_code)
  {
  case -1:
    puts("Unknown Error\r\n");
    break;
  case 1:
    puts(arg1);
    if(arg1_len) puts(" ");
    puts("File not found\r\n");
    break;
  case 2:
    puts(arg1);
    if(arg1_len) puts(" ");
    puts("Is a directory\r\n");
    break;
  case 3:
    puts(arg2);
    if(arg2_len) puts(" ");
    puts(" File already exists\r\n");
    break;
  case 4:
    puts("Storage is full\r\n");
    break;
  case 5:
    puts("Maximum file capacity achieved\r\n");
    break;
  case 6:
    puts("Maximum sector capacity achieved\r\n");
    break;
  case 7:
    puts(arg1);
    if(arg1_len) puts(" ");
    puts(arg2);
    if(arg2_len) puts(" ");
    puts("No such directory exists\r\n");
    break;
  case 8:
    puts(arg1);
    if(arg1_len) puts(" ");
    puts("Folder already exists\r\n");
    break;
  case 9:
    puts(arg1);
    puts(" Invalid arguments\r\n");
    break;
  default:
    break;
  }
}