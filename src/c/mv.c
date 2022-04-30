#include "header/filesystem.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/fileio.h"
#include "header/program.h"
#include "header/utils.h"
#include "header/message.h"
#include "header/string.h"

int main() {
  struct message m;
  struct node_filesystem node_fs_buffer;
  struct file_metadata fileinfo;
  char buffer[8192];
  int i = 0;
  int j = 0;
  byte ROOT = FS_NODE_P_IDX_ROOT;
  byte nodeFound = -1;
  byte dir = - 1;
  enum fs_retcode ret_code;

  getMessage(&m, getCurrentSegment());

  if (m.valid) {
    if (!checkArgs(m.arg2, &ret_code)) {
      error_code(9, m.arg1, m.arg2, m.arg3);
      exit();
    }

    clear(buffer, 8192);
    fileinfo.buffer = buffer;
    readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    for (i = 0; i < 64; i++) {
      if(node_fs_buffer.nodes[i].parent_node_index == m.current_directory && strcmp(node_fs_buffer.nodes[i].name, m.arg2)) {
        nodeFound = i;
        break;
      }
    }

    if (nodeFound == -1) {
      ret_code = FS_R_NODE_NOT_FOUND;
      error_code(ret_code, m.arg1, m.arg2, m.arg3);
      exit();
    }

    // if file found move it to fileinfo
    fileinfo.buffer = buffer;
    strcpy(fileinfo.node_name, m.arg2);
    fileinfo.parent_index = m.current_directory;

    //read file from filesystem
    read(&fileinfo, &ret_code);

    //if not found
    if (ret_code != FS_SUCCESS && ret_code != FS_R_TYPE_IS_FOLDER) {
      error_code(ret_code, m.arg1, m.arg2, m.arg3);
      exit();
    }

    //read root
    if (m.arg3[0] == '/') {
      fileinfo.parent_index = FS_NODE_P_IDX_ROOT;
      if (strlen(m.arg3 + 1) > 13) {
        ret_code = FS_W_NOT_ENOUGH_STORAGE;
        error_code(ret_code, m.arg1, m.arg2, m.arg3);
        exit();
      }

      if (m.arg3[1] != '\0') {
        strcpy(fileinfo.node_name, m.arg3 + 1);
      }

      node_fs_buffer.nodes[nodeFound].parent_node_index = FS_NODE_P_IDX_ROOT;
    } else if (m.arg3[0] == '.' && m.arg3[1] == '.' && m.arg3[2] == '/') {
      // read parent cwd
      if(strlen(m.arg3+3) > 13) {
        ret_code = FS_W_NOT_ENOUGH_STORAGE;
        error_code(ret_code, m.arg1, m.arg2, m.arg3);
        exit();
      }

      if(m.arg3[3] != '\0') {
        strcpy(fileinfo.node_name, m.arg3 + 3);
      }

      if (m.current_directory != ROOT) {
        ROOT = node_fs_buffer.nodes[m.current_directory].parent_node_index;
      }
      //set the file index to write and in the node
      fileinfo.parent_index = ROOT;
      node_fs_buffer.nodes[nodeFound].parent_node_index = ROOT;
    } else {
      //if in cur dir
      for (j = 0; j < 64; j++) {
        if (strcmp(node_fs_buffer.nodes[j].name, m.arg3) && m.current_directory == node_fs_buffer.nodes[j].parent_node_index) {
          if (node_fs_buffer.nodes[j].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
            dir = j;
            break;
          }
        }
      }
      
      if (dir == - 1) {
        ret_code = FS_W_INVALID_FOLDER;
        error_code(ret_code, m.arg1, m.arg2, m.arg3);
        exit();
      }

      if(strlen(m.arg3) > 13) {
        ret_code = FS_W_NOT_ENOUGH_STORAGE;
        error_code(ret_code, m.arg1, m.arg2, m.arg3);
        exit();
      }

      fileinfo.parent_index = dir;
      node_fs_buffer.nodes[nodeFound].parent_node_index = dir;
    }

    strcpy(node_fs_buffer.nodes[nodeFound].name, fileinfo.node_name);
    writeSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    // ret_code = FS_SUCCESS;

  } else {
    error_code(9, m.arg1, m.arg2, m.arg3);
  }

  exit();
}