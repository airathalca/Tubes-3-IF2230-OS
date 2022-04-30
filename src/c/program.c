#include "header/program.h"

//exec tuh intinya ngejalanin program di segment tertentu
void exec(struct message *m, int segment){
  struct file_metadata file;
  file.node_name = m->arg1;
  if(m->arg1[0] == '.' && m->arg1[1] == '/') file.parent_index = m->current_directory;
  else file.parent_index = 0;
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
  
  puts("DARI: ");
  putsHexa(getCurrentSegment());
  puts("\r\n");
  puts("MENUJU: ");
  putsHexa(now.next_program_segment);
  puts("\r\n");
  // puts(next.arg1);
  // puts("\r\n");
  exec(&next, now.next_program_segment);
}

byte read_relative_path(byte parentIdx, char *path_str, enum fs_retcode *ret_code) {
  char temp_str[128];
  struct node_filesystem node_fs_buffer;
  int i = 0;
  int j = 0;
  int k;
  bool found = false;
  byte prevParentIndex = parentIdx;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  clear(temp_str, 128);

  while (path_str[i] != '\0') {
    if(path_str[i] == '/') {
      i++; 
      continue;
    }

    j = 0;
    while (path_str[i] != '\0' && path_str[i] != '/'){
      temp_str[j] = path_str[i];
      i++;
      j++;
    }

    if (strcmp(temp_str, ".")) {
      clear(temp_str, 128);

    } else if (strcmp(temp_str, "..")) {
      if (parentIdx == FS_NODE_P_IDX_ROOT){
        *ret_code = FS_W_INVALID_FOLDER;
        return prevParentIndex;
      }

      parentIdx = node_fs_buffer.nodes[parentIdx].parent_node_index;
      clear(temp_str, 128);

    } else {
      found = false;
      for (k = 0; k < 64 && !found; k++){
        if(strcmp(temp_str, node_fs_buffer.nodes[k].name) && node_fs_buffer.nodes[k].parent_node_index == parentIdx){ // SALAH DISINI
          parentIdx = k;
          found = true;
        }
      }

      if(node_fs_buffer.nodes[parentIdx].sector_entry_index != FS_NODE_S_IDX_FOLDER) {
        *ret_code = FS_R_TYPE_IS_FOLDER;
        return prevParentIndex;

      } else if (!found){
        *ret_code = FS_W_INVALID_FOLDER;
        return prevParentIndex;

      } else {
        clear(temp_str, 128);
      }
    }
  }

  *ret_code = FS_SUCCESS;
  return parentIdx;
}

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

void error_code(int err_code, char *command, char *arg1, char *arg2){
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