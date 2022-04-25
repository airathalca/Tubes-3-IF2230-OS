#include "header/fileio.h"

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
  struct node_filesystem   node_fs_buffer;
  struct node_entry        node_buffer;
  struct sector_filesystem sector_fs_buffer;
  struct sector_entry      sector_entry_buffer;
  int i = 0;
  int j = 0;
  int foundEntryNode = FS_NODE_P_IDX_ROOT;
  bool found = false;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  readSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER);

  while (i < 64 && !found) {
    if (strcmp(node_fs_buffer.nodes[i].name, metadata->node_name) && metadata->parent_index == node_fs_buffer.nodes[i].parent_node_index){
      foundEntryNode = i;
      found = true;
    } else {
      i++;
    }
  }

  if (!found){
    *return_code = FS_R_NODE_NOT_FOUND;
    return;
  }

  if (node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
    metadata->filesize = 0;
    *return_code = FS_R_TYPE_IS_FOLDER;
    return;
  }
  memcpy(&sector_entry_buffer, &sector_fs_buffer.sector_list[node_fs_buffer.nodes[foundEntryNode].sector_entry_index], 16);
  for(j = 0; j < 16; j++){
    if(sector_entry_buffer.sector_numbers[j] != '\0'){
      readSector(metadata->buffer + (j*512), sector_entry_buffer.sector_numbers[j]);
    } else {
      break;
    }
  }
  metadata->filesize =  j * 512;
  *return_code = FS_SUCCESS;
}

void write(struct file_metadata *metadata, enum fs_retcode *return_code) {
  struct node_filesystem   node_fs_buffer;
  struct sector_filesystem sector_fs_buffer;
  struct map_filesystem    map_fs_buffer;
  int emptyNode = 0;
  int emptySector = 0;
  int i = 0;
  int j = 0;
  struct sector_entry buffer;
  bool found = false;
  bool isFile = false;
  unsigned int sizenow;

  readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  readSector(&sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);

  while (i < 64 && !found) {
    if (strcmp(node_fs_buffer.nodes[i].name, metadata->node_name) && 
    metadata->parent_index == node_fs_buffer.nodes[i].parent_node_index){
      found = true;
      if(node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) isFile = false;
      else isFile = true;
      break;
    }
    i++;
  }
  if (found) {
    if(isFile) *return_code = FS_W_FILE_ALREADY_EXIST;
    else *return_code = FS_W_FOLDER_ALREADY_EXISTS;
    return;
  }
  if (strlen(metadata->node_name) > 13) {
    *return_code = FS_W_NOT_ENOUGH_STORAGE;
    return;
  }

  for (j = 0; j < 64; j++) {
    if (node_fs_buffer.nodes[j].name[0] == '\0') {
      emptyNode = j;
      break;
    }
  }
  if (j == 64) {
    *return_code = FS_W_MAXIMUM_NODE_ENTRY;
    return;
  }

  if (metadata->parent_index != FS_NODE_P_IDX_ROOT && node_fs_buffer.nodes[metadata->parent_index].sector_entry_index != FS_NODE_S_IDX_FOLDER) {
    *return_code = FS_W_INVALID_FOLDER;
    return;
  }

  if (metadata->filesize > 8192) {
    *return_code = FS_W_NOT_ENOUGH_STORAGE;
    return;
  }
  sizenow = metadata->filesize;
  for (i = 0; i < 512; i++) {
    if (sizenow <= 0) {
      break;
    }
    if (!map_fs_buffer.is_filled[i]) {
      sizenow -= 512;
    }
  }
  if (sizenow > 0) {
    *return_code = FS_W_NOT_ENOUGH_STORAGE;
    return;
  }

  if(metadata->filesize > 0){
    for (i = 0; i < 32; i++) {
      if (sector_fs_buffer.sector_list[i].sector_numbers[0] == '\0') {
        emptySector = i;
        break;
      }
    }
    if (i == 32) {
      *return_code = FS_W_MAXIMUM_SECTOR_ENTRY;
      return;
    }
  }

  if (metadata->filesize == 0) {
    node_fs_buffer.nodes[emptyNode].sector_entry_index = FS_NODE_S_IDX_FOLDER;
  } 
  else {
    i = 0;
    for (j = 16; j < 256; j++) {
      if (map_fs_buffer.is_filled[j]) {
        continue;
      }
      map_fs_buffer.is_filled[j] = true;
      buffer.sector_numbers[i] = j;
      i++;
      if (metadata->filesize <= 512 * i) {
        break;
      }
    }
    for (j = 0; j < i; j++) {
      writeSector(metadata->buffer + j * 512, buffer.sector_numbers[j]);
    }
    memcpy(&sector_fs_buffer.sector_list[emptySector], &buffer, i);
    node_fs_buffer.nodes[emptyNode].sector_entry_index = emptySector;
    writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
    writeSector(&sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);
  }
  strcpy(node_fs_buffer.nodes[emptyNode].name, metadata->node_name);
  node_fs_buffer.nodes[emptyNode].parent_node_index = metadata->parent_index;
  writeSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  writeSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  *return_code = FS_SUCCESS;
  return;
}