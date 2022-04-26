#include "header/program.h"


void exec(struct message *m){
  struct file_metadata file;
  file.node_name = m->arg1;
  file.parent_index = m->current_directory;
  interrupt(0x21, 0x6, &file, 0x3000 + 0x1000 * m->program_counter, 0);
}
void exit(struct message *m){
  if(m->next_program_segment == 0){
    interrupt(0x21, 0x8, 0x2000, 0, 0);
  } else {
    exec(m);
  }
}