#include "header/filesystem.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/fileio.h"
#include "header/program.h"
#include "header/utils.h"
#include "header/message.h"
#include "header/string.h"

int main(){
  struct message m;
  struct node_filesystem node_fs_buffer;
  int i = 0;
  byte parentFound = FS_NODE_P_IDX_ROOT;
  enum fs_retcode ret_code;

  readSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  getMessage(&m, getCurrentSegment());
  puts("\r\n");
  puts(m.arg1);
  puts("\r\n");
  puts(m.arg2);
  puts("\r\n");
  puts(m.arg3);
  puts("\r\n");
  putsHexa(m.current_directory);
  puts("\r\n");
  putsHexa(getCurrentSegment());
  puts("\r\n");
  putsHexa(m.next_program_segment);
  puts("\r\n");
  puts(m.other);
  puts("\r\n");

  m.next_program_segment = 0x2000;
  sendMessage(&m, getCurrentSegment());
  exit();

//   if(m.arg2[0] == '\0'){
//     while (i < 64) {
//       if (strlen(node_fs_buffer.nodes[i].name) > 0 && node_fs_buffer.nodes[i].parent_node_index == m.current_directory) {
//         puts(node_fs_buffer.nodes[i].name);
//         puts("\r\n");
//       }
//       i++;
//     }
//     ret_code = FS_SUCCESS;
//     return;
//   }

//   // nyari nama nodes nya
//   for(i = 0; i < 64; i++) {
//     if (strlen(node_fs_buffer.nodes[i].name) > 0 && strcmp(node_fs_buffer.nodes[i].name, m.arg2) && node_fs_buffer.nodes[i].parent_node_index == m.current_directory){
//       parentFound = i;
//       break;
//     }
//   }

//   if(i == 64){
//     ret_code = FS_W_INVALID_FOLDER;
//     return;
//   }
}
