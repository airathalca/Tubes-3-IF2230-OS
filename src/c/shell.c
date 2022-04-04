#include "header/shell.h"

void shell() {
  char input_buf[64];
  char path_str[128];
  char command[32];
  char arg1[32];
  char arg2[32];
  int ret_code = 0;
  byte current_dir = FS_NODE_P_IDX_ROOT;
  while (true) {
    clear(input_buf, 64);
    clear(arg1, 32);
    clear(arg2, 32);
    clear(command, 32);
    printString("user@uSUSbuntuOS:");
    printCWD(path_str, current_dir);
    printString("$ ");
    readString(input_buf);
    argSplitter(&input_buf, &command, &arg1, &arg2);
    command_type(&command, &current_dir, &arg1, &arg2, &ret_code);
  }
}

void command_type(char *command, byte *current_dir, char*arg1, char* arg2, int *ret_code){
  if (strcmp(command, "cd")) {
    cd(current_dir, arg1, &ret_code);
  } 

  else if(strcmp(command, "clear")){
    clearScreen();
  }

  else if (strcmp(command, "ls")) {
      ls(*current_dir, arg1, &ret_code);
  } 

  else if (strcmp(command, "mv")) {
    mv(current_dir, "test", "test2", &ret_code);
  }

  else if (strcmp(command, "mkdir")) {
    mkdir(current_dir, arg1, &ret_code);
  } 

  else if (strcmp(command, "cat")) {
    cat(*current_dir, arg1, &ret_code);
  } 
  //aira
  else if (strcmp(command, "cp")) {
    cp(current_dir, "a", "b");
  }
  else {
      printString("Unknown command\r\n");
  }

  error_code(ret_code);
}

void argSplitter(char *input_buf, char *command, char* arg1, char *arg2){
  int size;
  int i = 0;
  int now = 0;
  int k;
  int count = 0;
  while(input_buf[i] != '\0') {
    if(count == 0){
      k = 0;
      while(input_buf[i] != ' ' && input_buf[i] != '\0'){
        command[k] = input_buf[i];
        k++;
        i++;
      }
      count++;
    }else if(count == 1){
      k = 0;
      while(input_buf[i] != ' ' && input_buf[i] != '\0'){
        arg1[k] = input_buf[i];
        k++;
        i++;
      }
      count++;
    }else if(count == 2){
      k = 0;
      while(input_buf[i] != ' ' && input_buf[i] != '\0'){
        arg2[k] = input_buf[i];
        k++;
        i++;
      }
      count++;
    }
    i++;
  }
}


void cd(byte *parentIndex, char *dir, int *ret_code) {
  struct node_filesystem node_fs_buffer;
  char temp_str[128];
  int i;
  bool found = false;
  int cur_idx = *parentIndex;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

	clear(temp_str, 128);
  
  // kalo gaada dir nya
  if(dir[0] == '\0'){
    *ret_code = FS_R_NODE_NOT_FOUND;
    return;
  }

  //masalah absolute pathing
  if(dir[0] == '/'){
    //placeholder
    *ret_code = FS_W_NOT_ENOUGH_STORAGE;
    return;
  }

  //balik ke parent
  if(dir[0] == '.' && dir[1] == '.'){
    if(cur_idx == FS_NODE_P_IDX_ROOT){
      *ret_code = FS_W_INVALID_FOLDER;
      return;
    }
    *parentIndex = node_fs_buffer.nodes[cur_idx].parent_node_index;
    *ret_code = FS_SUCCESS;
    return;
  }
  //if ada dir nya
  for(i = 0; i < 64; i++) {
    if(strcmp(node_fs_buffer.nodes[i].name, dir) && node_fs_buffer.nodes[i].parent_node_index == cur_idx){
      *parentIndex = i;
      break;
    }
  }

  *ret_code = FS_SUCCESS;
  return;

	// while (*dir != '\0') {
	// 	if (dir[0] == '/') {
	// 		i = 0;
	// 		while (i < 64 && !found) {
	// 			if (node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER && node_fs_buffer.nodes[i].parent_node_index == *parentIndex && strcmp(node_fs_buffer.nodes[i].name, temp_str)) {
	// 				found = true;
	// 			} else {
	// 				i++;
	// 			}
	// 		}

	// 		if (found) {
	// 			*parentIndex = i;

	// 		} else {
	// 			error_code(7);
	// 			break;
	// 		}

	// 		clear(temp_str, 128);

	// 	} else {
	// 		temp_str[strlen(temp_str)] = *dir;
	// 	}

	// 	dir++;
	// }
}

void ls(byte parentIdx, char* arg1, int *ret_code) {
  struct node_filesystem node_fs_buffer;
  int i = 0;
  byte parentFound = FS_NODE_P_IDX_ROOT;
  //klo emg di root trs kosong print gaada aja
  //klo emg mau nge ls tp gaada baru retcode
  //cek dulu apakah arg1 nya ga nol?
  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  if(arg1[0] == '\0'){
    while (i < 64) {
      if (strlen(node_fs_buffer.nodes[i].name) > 0 && node_fs_buffer.nodes[i].parent_node_index == parentIdx) {
        printString(node_fs_buffer.nodes[i].name);
        printString("\r\n");
      }
      i++;
    }
    if(i == 64){
      *ret_code = FS_R_NODE_NOT_FOUND;
    }
    return;
  }
  //kalo arg1 nya ga nol berarti ini nyari dulu node yg namanya sama
  for(i = 0; i < 64; i++) {
    if (strlen(node_fs_buffer.nodes[i].name) > 0 && strcmp(node_fs_buffer.nodes[i].name, arg1) && node_fs_buffer.nodes[i].parent_node_index == parentIdx){
      parentFound = i;
      break;
    }
  }

  if(i == 64){
    *ret_code = FS_R_NODE_NOT_FOUND;
    return;
  }

  //cari lagi sesuai parent idx yang baru
  for (i = 0; i < 64; i++) {
    if (strlen(node_fs_buffer.nodes[i].name) > 0 && node_fs_buffer.nodes[i].parent_node_index == parentFound){
      printString(node_fs_buffer.nodes[i].name);
      printString("\r\n");
    }
  }
}

void mv(byte parentIdx, char *source, char *target, int *ret_code) {
	struct node_filesystem node_fs_buffer;
  struct file_metadata fileinfo;
	int i = 0;
	bool found;
	byte addressSrc, addressTarget;

  read(&fileinfo, ret_code);
  if (*ret_code != 0){
    return;
  }

	readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

	while (i < 64 && !found) {
		if (node_fs_buffer.nodes[i].parent_node_index == parentIdx && strcmp(node_fs_buffer.nodes[i].name, source)) {
			found = true;
		} else {
			i++;
		}
	}

	if (found) {
    fileinfo.parent_index = addressTarget;
    write(&fileinfo, ret_code);

	} else {
    *ret_code = FS_W_INVALID_FOLDER;
		return;
	}
}

void cat(byte parentIndex, char *filename, int *ret_code) {
  //diketahui parentIndexnya trs tinggal searching node mana yang p nya sama 
  // berarti itu ada di folder tsb cek namanya sama apa ga
  struct file_metadata *fileInfo;
  byte text[512];
  int i = 0;
  if(filename[0] == '\0'){
    *ret_code = FS_R_NODE_NOT_FOUND;
    return;
  }
  fileInfo->parent_index = parentIndex;
  if(strlen(filename) > 13){
    *ret_code = FS_W_NOT_ENOUGH_STORAGE;
    return;
  }
  strcpy(fileInfo->node_name, filename);

  read(fileInfo, &ret_code);
  // printString(&fileInfo.node_name);
  // printString(&fileInfo.buffer);
  // for(i = 0; i < strlen(&fileInfo.buffer); i++){
  //   readSector(&text, fileInfo.buffer[i]);
  //   printString(text);
  // }
}

void mkdir(byte cur_dir_idx, char *arg1, int *ret_code){
  //cek dulu apakah ada folder yang namanya sama
  struct file_metadata fileinfo;
  fileinfo.parent_index = cur_dir_idx;
  fileinfo.filesize = 0;
  strcpy(&fileinfo.node_name, arg1);
  //udah ada isinya si fileinfonya;
  write(&fileinfo, ret_code);
}

void cp(byte parentIndex, char *resourcePath, char *destinationPath) {
  struct file_metadata fileInfo;
	int ret_code = 0;
  struct node_filesystem node_fs_buffer;
  int i = 0;

  fileInfo.parent_index = parentIndex;
  strcpy(fileInfo.node_name, resourcePath);
  read(&fileInfo, &ret_code);
  error_code(ret_code);
  if (ret_code != 0){
    return;
  }

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  while (i < 64) {
    if (node_fs_buffer.nodes[i].parent_node_index == parentIndex &&
    strcmp(node_fs_buffer.nodes[i].name, destinationPath) &&
    node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
      fileInfo.parent_index = i;
      break;
    }
    i++;
  }

  if (i == 64) {
    printString("Folder Tidak Ditemukan.\n");
    return;
  }
  write(&fileInfo,&ret_code);
}

void printCWD(char* path_str, byte current_dir) {
  //current_dir ini udah byte node anjir
  int i = 0;
  int nodeCount = 0;
  int curlen = 0;
  byte nodeIndex[64];
  byte parent = 0;
  byte filename[16];
  struct node_filesystem node_fs_buffer;
  struct sector_filesystem sector_fs_buffer;

  readSector(&node_fs_buffer, FS_NODE_SECTOR_NUMBER);
  readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 1);
  readSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER);
  //kosongin dulu
  clear(path_str, 128);
  clear(nodeIndex, 64);
  if(current_dir == FS_NODE_P_IDX_ROOT){
    path_str[curlen++] = '/';
    printString(path_str);
    return;
  }
  //masukan current dir ke array of node index
  //misal skrg di /a/b
  // /a/b/c
  nodeIndex[nodeCount] = current_dir;
  nodeCount++;
  //loop sampe parentnya oxFF
  parent = node_fs_buffer.nodes[current_dir].parent_node_index;
  while(parent != FS_NODE_P_IDX_ROOT){
    nodeIndex[nodeCount] = parent;
    parent = node_fs_buffer.nodes[parent].parent_node_index;
    nodeCount++;
  }
  
  while(nodeCount > 0){
    nodeCount--;
    path_str[curlen++] = '/';
    strcpy(filename, node_fs_buffer.nodes[nodeIndex[nodeCount]].name);
    for(i = 0; i < strlen(filename); i++){
      path_str[curlen++] = filename[i];
    }
  }
  printString(path_str);
}

// byte readPath(char *path_str, struct node_filesystem node_fs_buffer, struct sector_filesystem sector_fs_buffer){
//   char *node_name;
//   int node_idx;
//   int parent_idx = current_dir;

//   for(node_idx = 0; node_idx < 64; node_idx++){
//     if(node_fs_buffer.nodes[node_idx].parent_node_index == current_dir){
//       break;
//     }
//   }

//   path_str[0] = '/';
//   //klo root
//   if(parent_idx == FS_NODE_P_IDX_ROOT){
    
//     printString(path_str);
//     return;
//   }

//   strcpy(node_name, node_fs_buffer.nodes[node_idx].name);

// }

byte read_absolute_path(char *path_str, enum fs_retcode *ret_code) {
  char temp_str[128];
  struct node_filesystem node_fs_buffer;
  int i;
  bool found;
  byte parentIdx = FS_NODE_P_IDX_ROOT;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  clear(temp_str, 128);

  while (*path_str != '\0') {
    if (*path_str == '/' && temp_str != '\0') {
      i = 0;
      found = false;

      while (i < 64 && !found) {
        if (node_fs_buffer.nodes[i].parent_node_index == parentIdx && strcmp(node_fs_buffer.nodes[i].name, path_str)) {
          found = true;
        } else {
          i++;
        }
      }

      if (found) {
        parentIdx = i;

      } else {
        *ret_code = FS_W_INVALID_FOLDER;
		    return;
      }

      clear(temp_str, 128);
    }

    temp_str[strlen(temp_str)] = *path_str;
    *path_str++;
  }

  return parentIdx;
}

void error_code(int err_code){
  switch (err_code)
  {
  case -1:
    printString("Unknown Error\r\n");
    break;
  case 2:
    printString("This is directory not file\r\n");
    break;
  case 3:
    printString("File already exists\r\n");
    break;
  case 4:
    printString("Storage is full\r\n");
    break;
  case 7:
    printString("No such file or directory\r\n");
    break;
  default:
    break;
  }
}

void printFile()