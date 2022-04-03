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
    cd(&current_dir, "a");
  } 

  else if(strcmp(command, "clear")){
    clearScreen();
  }

  else if (strcmp(command, "ls")) {
      ls(current_dir, arg1, ret_code);
  } 

  else if (strcmp(command, "mv")) {
    mv(current_dir, "test", "test2");
  } 

  else if (strcmp(command, "mkdir")) {
    mkdir(current_dir, arg1, &ret_code);
  } 

  else if (strcmp(command, "cat")) {
    cat(current_dir, arg1, &ret_code);
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


void cd(byte *parentIndex, char *dir) {
  struct node_filesystem node_fs_buffer;
  char temp_str[128];
  int i;
  bool found = false;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

	clear(temp_str, 128);

	while (*dir != '\0') {
		if (*dir == '/') {
			i = 0;
			while (i < 64 && !found) {
				if (node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER && node_fs_buffer.nodes[i].parent_node_index == *parentIndex && strcmp(node_fs_buffer.nodes[i].name, temp_str)) {
					found = true;
				} else {
					i++;
				}
			}

			if (found) {
				*parentIndex = i;

			} else {
				error_code(7);
				break;
			}

			clear(temp_str, 128);

		} else {
			temp_str[strlen(temp_str)] = *dir;
		}

		dir++;
	}
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
      if (node_fs_buffer.nodes[i].parent_node_index == parentIdx) {
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
    if (strcmp(node_fs_buffer.nodes[i].name, arg1)){
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
    if (node_fs_buffer.nodes[i].parent_node_index == parentFound){
      printString(node_fs_buffer.nodes[i].name);
      printString("\r\n");
    }
  }
}

void mv(byte parentIdx, char *source, char *target) {
	struct node_filesystem node_fs_buffer;
	int i = 0;
	bool found;
	byte addressSrc, addressTarget;

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
		addressSrc = i;

	} else {
		error_code(7);
		return;
	}

	cd(&addressTarget, target); // target berbentuk absolute dir

	node_fs_buffer.nodes[i].parent_node_index = addressTarget;
}

void cat(byte parentIndex, char *filename, int *ret_code) {
  //diketahui parentIndexnya trs tinggal searching node mana yang p nya sama 
  // berarti itu ada di folder tsb cek namanya sama apa ga
  struct file_metadata fileInfo;

  if(filename[0] == '\0'){
    *ret_code = FS_R_NODE_NOT_FOUND;
    return;
  }

  fileInfo.parent_index = parentIndex;
  memcpy(&fileInfo.node_name, filename, strlen(filename));
  read(&fileInfo, &ret_code);
  printString(&fileInfo.buffer);
}

void mkdir(byte cur_dir_idx, char *arg1, int *ret_code){
  //cek dulu apakah ada folder yang namanya sama
  struct file_metadata fileinfo;
  fileinfo.parent_index = cur_dir_idx;
  fileinfo.filesize = 0;
  memcpy(&fileinfo.node_name, arg1, strlen(arg1));
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
  struct node_filesystem node_fs_buffer;
  struct sector_filesystem sector_fs_buffer;

  readSector(&node_fs_buffer, FS_NODE_SECTOR_NUMBER);
  readSector(&node_fs_buffer.nodes[32], FS_NODE_SECTOR_NUMBER + 1);
  readSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER);
  //kosongin dulu
  clear(path_str, 128);
  path_str[curlen++] = '/';
  if(current_dir == FS_NODE_P_IDX_ROOT){
    printString(path_str);
    return;
  }
  //masukan current dir ke array of node index
  nodeIndex[nodeCount++] = current_dir;
  // misal skrg ga di root tp di /a/b/c
  //loop sampe parentnya oxFF
  parent = node_fs_buffer.nodes[current_dir].parent_node_index;
  while(parent != FS_NODE_P_IDX_ROOT){
    nodeIndex[nodeCount++] = parent;
    parent = node_fs_buffer.nodes[parent].parent_node_index;
  }
  //parent udah 0xFF dan skrg nodeIndex berisi node dari awal sampe akhir tp kebalik
  //copy namanya dari awal sampe akhir curlen skrg udah 1
  while(nodeCount > 0){
    strcpy(path_str + curlen, node_fs_buffer.nodes[nodeIndex[nodeCount]].name);
    nodeCount--;
    curlen += strlen(node_fs_buffer.nodes[nodeIndex[nodeCount]].name);
    if(nodeCount > 0){
      path_str[curlen++] = '/';
    }
  }
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