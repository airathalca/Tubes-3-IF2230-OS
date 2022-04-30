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
  struct file_metadata fileinfo;
  char buffer[8192];
  enum fs_retcode ret_code;

  clear(buffer, 8192);

  getMessage(&m, getCurrentSegment());

  if (m.valid) {
    if(m.arg3[0] != '\0') {
      error_code(9, m.arg1, m.arg2, m.arg3);
      exit();
    }
    
    if (!checkArgs(m.arg2, &ret_code)) {
      sendMessage(&m, getCurrentSegment());
      error_code(9, m.arg1, m.arg2, m.arg3);
      exit();
    }

    fileinfo.parent_index = m.current_directory;
    fileinfo.buffer = buffer;
    strcpy(fileinfo.node_name, m.arg2);
    
    read(&fileinfo, &ret_code);
    if(ret_code == 0) {
      puts(fileinfo.buffer);
      puts("\r\n");
    } else {
      error_code(ret_code, m.arg1, m.arg2, m.arg3);
    }

  } else {
    error_code(9, m.arg1, m.arg2, m.arg3);
  }

  exit();
}