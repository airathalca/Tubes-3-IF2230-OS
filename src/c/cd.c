#include "header/filesystem.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/fileio.h"
#include "header/program.h"
#include "header/utils.h"

int main() {
  struct message m;
  struct node_filesystem node_fs_buffer;
  enum fs_retcode ret_code;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  getMessage(&m, getCurrentSegment());

  if (m.valid) {
    if(m.arg3[0] != '\0') {
      error_code(9, m.arg1, m.arg2, m.arg3);
      exit();
    }
    
    if (m.arg2[0] == '\0') {
      ret_code = FS_R_NODE_NOT_FOUND;
      error_code(ret_code, m.arg1, m.arg2, m.arg3);

    } else if (m.arg2[0] == '/') {
      m.current_directory = read_relative_path(FS_NODE_P_IDX_ROOT, m.arg2 + 1, &ret_code);

      if (ret_code != 0) {
        error_code(ret_code, m.arg1, m.arg2, m.arg3);
      }

    } else {
      m.current_directory = read_relative_path(m.current_directory, m.arg2, &ret_code);

      if (ret_code != 0) {
        error_code(ret_code, m.arg1, m.arg2, m.arg3);
      }
    }

  } else {
    error_code(9, m.arg1, m.arg2, m.arg3);
  }

  sendMessage(&m, getCurrentSegment());
  exit();
}