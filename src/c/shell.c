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
    mv(*current_dir, arg1, arg2, &ret_code);
  }

  else if (strcmp(command, "mkdir")) {
    mkdir(*current_dir, arg1, &ret_code);
  } 

  else if (strcmp(command, "cat")) {
    cat(*current_dir, arg1, &ret_code);
  } 
  //aira
  else if (strcmp(command, "cp")) {
    cp(current_dir, "a", "b", &ret_code);
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
  int i;
  int cur_idx = *parentIndex;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  
  // kalo gaada dir nya
  if(dir[0] == '\0'){
    *ret_code = FS_R_NODE_NOT_FOUND;
    return;
  }

  //masalah absolute pathing
  if(dir[0] == '/'){
    *parentIndex = read_absolute_path(dir, ret_code);
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

void mv(byte parentIndex, char *source, char *target, int *ret_code) {
	struct node_filesystem node_fs_buffer;
  struct file_metadata *fileinfo;
	int i = 0;
	byte addressSrc, addressTarget;
  byte ROOT = FS_NODE_P_IDX_ROOT;
  byte nodeFound = 0;
  bool alreadySame = false;
  bool isFile = false;
  bool found = false;

	readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  printString("1");

  if(source[0] == '\0' || target[0] == '\0'){
    //masukin error bahwa mv harus make argunmen
    *ret_code = FS_R_NODE_NOT_FOUND;
    return;
  }

  // cari dulu node yang ounya nama sama kaya siini
  for(i = 0; i < 64; i++){
    if(node_fs_buffer.nodes[i].parent_node_index == parentIndex && strcmp(node_fs_buffer.nodes[i].name, source)){
      nodeFound = i;
      found = true;
      break;
    }
  }
  if(!found){
    *ret_code = FS_R_NODE_NOT_FOUND;
    return;
  }

    printString("2");

  // //isi fileinfo
  fileinfo->parent_index = parentIndex;
  printHexa(fileinfo->parent_index);
   if(strlen(source) > 13){
    *ret_code = FS_W_NOT_ENOUGH_STORAGE;
    return;
  }

  printString("3");

  strcpy(fileinfo->node_name, source);
  // printString("4");
  read(fileinfo, ret_code);

  // //folder udah kesisi tinggal cek ada yang namanya sama ga di root
  // //skrg bisa mindahin ke folder atau root atau abs
  // //root
  if(target[0] == '/'){
    printString("AA");
    fileinfo->parent_index = FS_NODE_P_IDX_ROOT;
    if(target[1] != '\0'){
      strcpy(fileinfo->node_name, target+1);
    }
    printString(fileinfo->node_name);
    node_fs_buffer.nodes[nodeFound].parent_node_index = fileinfo->parent_index;
  }
  else if(target[0] == '.' && target[1] == '.' && target[2] == '/'){
    //cari juga nama yang sama
    if(parentIndex != FS_NODE_P_IDX_ROOT){
      ROOT = node_fs_buffer.nodes[parentIndex].parent_node_index;
    }
    if(target[3] != '\0'){
      strcpy(fileinfo->node_name, target+3);
    }
    printString(fileinfo->node_name);
    fileinfo->parent_index = ROOT;
    node_fs_buffer.nodes[nodeFound].parent_node_index = ROOT;
  }else{
    // pindahin ke dalem folder cari index si foldernya
    for(i = 0; i < 64; i++){
      if(node_fs_buffer.nodes[i].parent_node_index == parentIndex && strcmp(node_fs_buffer.nodes[i].name, target)){
        ROOT = i;
        break;
      }
    }
    if(i == 64){
      *ret_code = FS_R_NODE_NOT_FOUND;
      return;
    }
    fileinfo->parent_index = ROOT;
    node_fs_buffer.nodes[nodeFound].parent_node_index = ROOT;
    strcpy(fileinfo->node_name, target);
  }
  write(fileinfo, ret_code);
  writeSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  writeSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
}

void cat(byte parentIndex, char *filename, int *ret_code) {
  //diketahui parentIndexnya trs tinggal searching node mana yang p nya sama 
  // berarti itu ada di folder tsb cek namanya sama apa ga
  struct file_metadata *fileInfo;
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

  read(fileInfo, ret_code);
  if(*ret_code == 0){
    printString(fileInfo->buffer);
    printString("\r\n");
  }
}

void mkdir(byte cur_dir_idx, char *arg1, int *ret_code){
  //cek dulu apakah ada folder yang namanya sama
  struct file_metadata *fileinfo;
  int i = 0;
  if(arg1[0] == '\0'){
    *ret_code = FS_R_NODE_NOT_FOUND;
    return;
  }
  fileinfo->parent_index = cur_dir_idx;
  if(strlen(arg1) > 13){
    *ret_code = FS_W_NOT_ENOUGH_STORAGE;
    return;
  }
  strcpy(fileinfo->node_name, arg1);
  //udah ada isinya si fileinfonya;
  write(fileinfo, ret_code);
}

void cp(byte parentIdx, char *resourcePath, char *destinationPath, int *ret_code){
  struct file_metadata *fileInfo;
  struct node_filesystem  node_fs_buffer;
  int i = 0;
  int foundEntryNode;
  bool found = false;
  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  while (i < 64 && !found) {
    if (strcmp(node_fs_buffer.nodes[i].name, destinationPath) && parentIdx == node_fs_buffer.nodes[i].parent_node_index){
      if (node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
        foundEntryNode = i;
        found = true;
      }
    } else {
      i++;
    }
  }
  if (!found) {
    *ret_code = FS_W_INVALID_FOLDER;
    return;
  }

  clear(fileInfo->buffer,8192);
  if(resourcePath[0] == '\0'){
    *ret_code = FS_R_NODE_NOT_FOUND;
    return;
  }
  fileInfo->parent_index = parentIdx;
  if(strlen(resourcePath) > 13){
    *ret_code = FS_W_NOT_ENOUGH_STORAGE;
    return;
  }
  strcpy(fileInfo->node_name, resourcePath);
  read(fileInfo, ret_code);
  if (*ret_code != FS_SUCCESS && *ret_code != FS_R_TYPE_IS_FOLDER){
    return;
  }
  fileInfo->parent_index = foundEntryNode;
  strcpy(fileInfo->node_name, resourcePath);
  write(fileInfo, ret_code);
  return;
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
  
  while(nodeCount > 0 && curlen < 128){
    nodeCount--;
    path_str[curlen++] = '/';
    strcpy(filename, node_fs_buffer.nodes[nodeIndex[nodeCount]].name);
    if(curlen + strlen(filename) > 128){
      break;
    }
    for(i = 0; i < strlen(filename); i++){
      path_str[curlen++] = filename[i];
    }
  }
  printString(path_str);
}

byte read_absolute_path(char *path_str, enum fs_retcode *ret_code) {
  char temp_str[128];
  struct node_filesystem node_fs_buffer;
  int i = 0;
  int j = 0;
  int k;
  bool found = false;
  byte parentIdx = FS_NODE_P_IDX_ROOT;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);


  clear(temp_str, 128);
  while (path_str[i] != '\0') {
    if(path_str[i] == '/'){i++; continue;}
    j = 0;
    while(path_str[i] != '\0' && path_str[i] != '/'){
      temp_str[j] = path_str[i];
      i++;
      j++;
    }
    for(k = 0; k < 64; k++){
      if(strcmp(node_fs_buffer.nodes[k].name, temp_str) && node_fs_buffer.nodes[k].parent_node_index == parentIdx){
        parentIdx = k;
        found = true;
        break;
      }
    }

    if(!found){
      *ret_code = FS_R_NODE_NOT_FOUND;
      return parentIdx;
    }
    clear(temp_str, 128);
    i++;
  }
  if(node_fs_buffer.nodes[parentIdx].sector_entry_index != FS_NODE_P_IDX_ROOT){
    //harusnya file typenya file jdi gabisa ini tinggal ganti retcode
    *ret_code = FS_R_TYPE_IS_FOLDER;
    return FS_NODE_P_IDX_ROOT;
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