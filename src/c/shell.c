#include "header/shell.h"

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

  //kinan
  if (strcmp(input_buf, "cd")) {
    
  } 
  //beres
  else if (strcmp(input_buf, "ls")) {
      
  } 
  //aing
  else if (strcmp(input_buf, "mv")) {

  } 
  //aing
  else if (strcmp(input_buf, "mkdir")) {
    
  } 
  //beres
  else if (strcmp(input_buf, "cat")) {

  } 
  //aira
  else if (strcmp(input_buf, "cp")) {

  } 
  else {
      printString("Unknown command\r\n");
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
  //1. mane cari yang current_dir nya udah index ngab
  nodeIndex[idx++] = current_dir;
  parent = node_fs_buffer.nodes[current_dir].parent_node_index;
  //simpen semuanya di nodeIndex
  while(parent != FS_NODE_P_IDX_ROOT){
    if (parent == (j % 64)) {
      nodeIndex[idx++] = j;
      parent = node_fs_buffer.nodes[j].parent_node_index;
    }
    j++;
  }
  //copy semuanya ke path_str
  while(idx >= 0) {
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
  strcpy(path_str + curLen, iteratorPath);
  printString(path_str);
}

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