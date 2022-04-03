#include "header/shell.h"

void shell() {
  char input_buf[64];
  char path_str[128];
  byte current_dir = FS_NODE_P_IDX_ROOT;
  char command[64];
  char arg1[64];
  char arg2[64];
  while (true) {
    printString("user@uSUSbuntu:");
    printCWD(path_str, current_dir);
    printString("$ ");
    readString(input_buf);
    argSplitter(&input_buf, &command, &arg1, &arg2);
    printString(input_buf);
    printString("\r\n");
    printString(command);
    printString("\r\n");
    printString(arg1);
    printString("\r\n");
    printString(arg2);
    printString("\r\n");
    // command(&input_buf, &current_dir);
  }
}

void command(char *input_buf, byte *current_dir){
  if (strcmp(input_buf, "cd")) {
    cd(&current_dir, "a");
  } 

  else if(strcmp(input_buf, "clear")){
    clearScreen();
  }

  else if (strcmp(input_buf, "ls")) {
      ls(current_dir);
  } 

  else if (strcmp(input_buf, "mv")) {
    mv(current_dir, "test", "test2");
  } 

  else if (strcmp(input_buf, "mkdir")) {
    struct file_metadata buffer;
    buffer.node_name[0] = 'a';
    mkdir(current_dir, &buffer);
  } 

  else if (strcmp(input_buf, "cat")) {
    cat(current_dir, "a");
  } 
  //aira
  else if (strcmp(input_buf, "cp")) {
    cp(current_dir, "a", "b");
  } 
  else {
      printString("Unknown command\r\n");
  }
}

void argSplitter(char *input_buf, char *command, char* arg1, char *arg2){
  int size;
  int i = 0;
  int now = 0;
  int k;
  int count = 0;
  clear(command, 64);
  clear(arg1, 64);
  clear(arg2, 64);
  while(input_buf[i] != '\0') {
    while(input_buf[i] == ' '){
      if(count == 0){
        k = 0;
        while(now < i) {
          command[k] = input_buf[now];
          k++;
          now++;
        }
      } else if (count == 1) {
        k = 0;
        while(now < i) {
          arg1[k] = input_buf[now];
          k++;
          now++;
        }
      }
      now++;
      i++;
      count++;
    }
    if(count == 2){
      k = 0;
      while(input_buf[now] != '\0'){
        arg2[k] = input_buf[now];
        k++;
        now++;
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

void ls(byte parentIdx) {
  struct node_filesystem node_fs_buffer;
  int i = 0;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    
  while (i < 64) {
    if (node_fs_buffer.nodes[i].parent_node_index == parentIdx && 
    node_fs_buffer.nodes[i].name[0] != '\0') {
      printString(node_fs_buffer.nodes[i].name);
      printString("\n");
    }
  i++;
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

void lsWithDir(byte parentIdx, char *path) {
  struct node_filesystem node_fs_buffer;
	int i = 0;

	readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  while (i < 64) {
    if (node_fs_buffer.nodes[i].parent_node_index == parentIdx &&
    strcmp(node_fs_buffer.nodes[i].name, path) &&
    node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
      ls(i);
      break;
    }
    i++;
  }
  if (i == 64) {
    printString("Folder Tidak Ditemukan.\n");
  }
}

void cat(byte parentIndex, char *filename) {
  //diketahui parentIndexnya trs tinggal searching node mana yang p nya sama berarti itu ada di folder tsb cek namanya sama apa ga
  struct file_metadata fileInfo;
	int ret_code = 0;

  fileInfo.parent_index = parentIndex;
  strcpy(fileInfo.node_name, filename);
  read(&fileInfo, &ret_code);
  error_code(ret_code);
  if(ret_code != 0){
    return;
  }

  printString(fileInfo.buffer);
  ///dapet sector number terus baca semuanya
}

void mkdir(byte cur_dir_idx, struct file_metadata *fileInfo){
  //udah ada isinya si fileinfonya;
  int ret_code;

  fileInfo->parent_index = cur_dir_idx;
  fileInfo->filesize = 0;
  write(fileInfo, &ret_code);
  error_code(ret_code);
  if(ret_code != 0){
    return;
  }
  printString("Folder berhasil dibuat");
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
    printString("Unknown Error\n");
    break;
  case 2:
    printString("This is directory not file\n");
    break;
  case 3:
    printString("File already exists\n");
    break;
  case 4:
    printString("Storage is full\n");
    break;
  case 7:
    printString("No such file or directory\n");
    break;
  default:
    break;
  }
}