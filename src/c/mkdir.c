#include "header/filesystem.h"
#include "header/std_type.h"
#include "header/textio.h"
#include "header/fileio.h"
#include "header/program.h"
#include "header/utils.h"

int main(){
  struct message m;
  struct file_metadata fileinfo;
  char buffer[8192];
  int i = 0;
  enum fs_retcode ret_code;

  getMessage(&m, getCurrentSegment());
  

  if (!checkArgs(m.arg2, &ret_code)) {
    sendMessage(&m, getCurrentSegment());
    exit();
  }
  puts(m.arg2);
  fileinfo.buffer = buffer;
  fileinfo.filesize = 0;
  strcpy(fileinfo.node_name, m.arg2);

  write(&fileinfo, &ret_code);
  sendMessage(&m, getCurrentSegment());
  exit();
}