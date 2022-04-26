#include "header/message.h"

void getMessage(struct message *m, int segment){
  interrupt(0x21, 0x2, m, 0x104 + m->program_counter, 0);
}

void sendMessage(struct message *m, int segment){
  interrupt(0x21, 0x3, m, 0x104 + m->program_counter, 0);
}