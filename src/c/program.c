#include "header/program.h"

//exec tuh intinya ngejalanin program di segment tertentu
void exec(struct message *m, int segment){
  struct file_metadata file;
  file.node_name = m->arg1;
  file.parent_index = m->current_directory;
  interrupt(0x21, 0x6, &file, segment, 0);
}

void exit(){
  struct message now;
  struct message next;

  //ambil message abis itu set next dir 
  getMessage(&now, getCurrentSegment());
  getMessage(&next, now.next_program_segment);
  next.current_directory = now.current_directory;
  sendMessage(&next, now.next_program_segment);


  exec(&next, now.next_program_segment);
};

