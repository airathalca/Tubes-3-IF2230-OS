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

  getMessage(&m, getCurrentSegment());

  if (!checkArgs(m.arg2, &ret_code)) {
    sendMessage(&m, getCurrentSegment());
    exit();
  }

  fileinfo.parent_index = m.current_directory;
  fileinfo.buffer = buffer;
  strcpy(fileinfo.node_name, m.arg2);
  
  read(&fileinfo, &ret_code);
  if(ret_code == 0) {
    puts(fileinfo.buffer);
    puts("\r\n");
  }

  sendMessage(&m, getCurrentSegment());
  exit();
}